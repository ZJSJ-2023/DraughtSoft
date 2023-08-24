#include "draughtsoft.h"
#include <QGraphicsProxyWidget>
#include "subinfowidget.h"
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include "customgraphicview.h"
#include "uifactory.h"
#include <QToolBar>
#include "customgraphicscene.h"
#include <QStyleOptionGraphicsItem>
#include "customgraphicpixmapitem.h"
#include "globalinstance.h"
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "httpfunc.h"
#include <functional>
#include "appsettings.h"
#include "CustomGraphicRectItem.h"
#include "itemmanager.h"
#include <QDebug>

DraughtSoft::DraughtSoft(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowState(Qt::WindowMaximized);

	GlobalInstance::getInstance().setMainWindow(this);

	QToolBar* toolBar = new QToolBar(this);
	addToolBar(Qt::LeftToolBarArea, toolBar);
	toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	toolBar->setMovable(false);
	toolBar->setFloatable(false);

	ControlPorperty property;
	QPushButton* pCreateFenceBtn = UiFactory::createControl<QPushButton>(property.init().setCheckableState(true).setText(tr("Create Fence")), this);
	QPushButton* pMoveCloudBtn = UiFactory::createControl<QPushButton>(property.init().setCheckableState(true).setText(tr("Move Cloud")), this);

	toolBar->addWidget(pCreateFenceBtn);
	toolBar->addWidget(pMoveCloudBtn);

	m_pixmapItem = new QGraphicsPixmapItem();
	m_pixmapItem->setPixmap(QPixmap(Appsettings::getImgPath() + "map.png"));
	//m_pixmapItem->setFlags(QGraphicsItem::ItemIsMovable);  // if set,the pixmap item can't double clicked
	m_pixmapItem->setPos(0, 0);

	GlobalInstance::getInstance().setRootItem(m_pixmapItem);

	scene = new CustomGraphicScene(this);
	scene->setStickyFocus(true);
	scene->addItem(m_pixmapItem);

	scene->setSceneRect(scene->itemsBoundingRect());

	m_pGraphicView = new CustomGraphicView(this);
	m_pGraphicView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	//m_pGraphicView->setDragMode(QGraphicsView::ScrollHandDrag);
	m_pGraphicView->setScene(scene);
	m_pGraphicView->scale(1, 1);
	m_pGraphicView->setRenderHints(m_pGraphicView->renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	m_pGraphicView->setAutoFillBackground(true);
	m_pGraphicView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	m_pGraphicView->move(0, 0);
	m_pGraphicView->show();

	connect(pCreateFenceBtn, &QPushButton::toggled, this, [=](bool check) {
		unCheckOhterButton(check, pCreateFenceBtn);

		scene->setCreateFence(check);

		m_pixmapItem->setFlags(check ? (m_pixmapItem->flags() & ~QGraphicsItem::ItemIsMovable)
							   : (m_pixmapItem->flags() | QGraphicsItem::ItemIsMovable));
	});

	connect(pMoveCloudBtn, &QPushButton::toggled, this, [=](bool check) {
		unCheckOhterButton(check, pMoveCloudBtn);

		for (auto item : scene->items())
		{
			CustomGraphicPixmapItem* pixmapItem = dynamic_cast<CustomGraphicPixmapItem*>(item);
			if (pixmapItem)
				pixmapItem->setFlags(check ? (pixmapItem->flags() | QGraphicsItem::ItemIsMovable)
									 : (pixmapItem->flags() & ~QGraphicsItem::ItemIsMovable));
		}

		/*m_pixmapItem->setFlags(check ? (m_pixmapItem->flags() & ~QGraphicsItem::ItemIsMovable)
							   : (m_pixmapItem->flags() | QGraphicsItem::ItemIsMovable));*/

							   // post pos when pos locked
		if (!check)
			postFFu();
	});

	setCentralWidget(m_pGraphicView);

	//updateItems("");

	// 第一次请求，初始化数据
	HttpFunc::request("http://localhost:8080/allInfo", std::bind(&DraughtSoft::updateItems, this, std::placeholders::_1, std::placeholders::_2));

	QTimer* pTimer = new QTimer(this);
	pTimer->setInterval(5000);
	pTimer->start();
	connect(pTimer, &QTimer::timeout, this, [=]() {
		HttpFunc::request("http://localhost:8080/allInfo", std::bind(&DraughtSoft::updateItems, this, std::placeholders::_1, std::placeholders::_2));
	});

	// 实时订阅tag位置信息
	m_pWebSocket = new CustomWebSocket(scene, m_pixmapItem);
	m_pWebSocket->startToConnect();
}

DraughtSoft::~DraughtSoft()
{
	postFences();
}

#define TEST
void DraughtSoft::updateItems(int error, const QString& str)
{
	if (error != 0)
	{
		qDebug() << "Network Error!";
		return;
	}

	QString jsonStr;

#ifdef TEST
	// just for test
	QJsonObject root;
	root.insert("isOK", 1);

	QJsonArray array;
	for (int i = 0; i < 5; i++)
	{
		QJsonObject ffu;
		ffu.insert("id", i);
		ffu.insert("speed", 30);
		ffu.insert("state", 0);

		array.push_back(ffu);
	}

	root.insert("FFU Array", array);

	jsonStr = QJsonDocument(root).toJson();
#else
	jsonStr = str;
#endif

	parseFFu(jsonStr);
	parseFence(jsonStr);

	scene->update();
	}

// post pos information to server
void DraughtSoft::postFFu()
{
	QJsonObject root;

	QJsonArray array;
	for (QGraphicsItem* pItem : m_pixmapItem->childItems())
	{
		CustomGraphicPixmapItem* pCustomItem = dynamic_cast<CustomGraphicPixmapItem*>(pItem);
		if (!pCustomItem)
			continue;

		QJsonObject ffu;
		ffu.insert("id", pCustomItem->getId());
		ffu.insert("x", pCustomItem->pos().x());
		ffu.insert("y", pCustomItem->pos().y());

		array.push_back(ffu);
	}

	root.insert("FFU Array", array);

	const QString jsonStr = QJsonDocument(root).toJson();

	HttpFunc::post("", jsonStr, [=](int error, const QString& str)->void {
		// do something 
	});
}

void DraughtSoft::postFences()
{
	QJsonObject root;

	QJsonArray array;
	for (QGraphicsItem* pItem : m_pixmapItem->childItems())
	{
		CustomGraphicRectItem* pCustomItem = dynamic_cast<CustomGraphicRectItem*>(pItem);
		if (!pCustomItem)
			continue;

		QJsonObject fence;
		fence.insert("name", pCustomItem->getName());
		fence.insert("type", pCustomItem->getType());
		fence.insert("color", pCustomItem->getColor());

		QRectF rect = pCustomItem->rect();

		fence.insert("x", rect.x());
		fence.insert("y", rect.y());
		fence.insert("width", rect.width());
		fence.insert("height", rect.height());

		array.push_back(fence);
	}

	root.insert("Fence Array", array);

	const QString jsonStr = QJsonDocument(root).toJson();

	HttpFunc::post("", jsonStr, [=](int error, const QString& str)->void {
		// do something 
	});
}

void DraughtSoft::parseFFu(const QString& str)
{
	QJsonObject root = QJsonDocument::fromJson(str.toUtf8().data()).object();

	QJsonArray array = root["FFU Array"].toArray();
	if (array.isEmpty())
		return;

	for (int i = 0; i < array.size(); ++i)
	{
		QJsonObject obj = array[i].toObject();
		int id = obj["id"].toInt();

		// 如果已经存在，那么只是刷新信息，如果不存在，那么加入进去
		CustomGraphicPixmapItem* pItem = nullptr;
		if (!ItemManagerInstance()->containsFFu(id))
		{
			pItem = new CustomGraphicPixmapItem(m_pixmapItem);
			pItem->setId(id);
			//pItem->update();

#ifdef TEST
			QRectF rect = pItem->boundingRect();
			int x = i % 5;
			int y = i / 5;
			pItem->setPos(x * rect.width() * 1.2, y * rect.height() * 1.2);
#else if
			pItem->setPos(obj["x"].toInt(), obj["y"].toInt());
#endif
			pItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

			ItemManagerInstance()->addFFu(id, pItem);
			scene->addItem(pItem);
		}
		else
			pItem = ItemManagerInstance()->getFFu(id);

		if (pItem)
		{
			pItem->setOnline(obj["state"].toInt());
			pItem->setSpeed(obj["speed"].toInt());
			pItem->update();
		}
	}
}

void DraughtSoft::parseFence(const QString& str)
{
	QJsonObject root = QJsonDocument::fromJson(str.toUtf8().data()).object();

	QJsonArray array = root["Fence Array"].toArray();
	if (array.isEmpty())
		return;

	for (int i = 0; i < array.size(); ++i)
	{
		QJsonObject obj = array[i].toObject();
		QString name = obj["name"].toString();
		QString type = obj["type"].toString();
		int color = obj["color"].toInt();

		// 如果已经存在，那么只是刷新信息，如果不存在，那么加入进去
		CustomGraphicRectItem* pItem = nullptr;
		if (!ItemManagerInstance()->containseFence(name))
		{
			pItem = new CustomGraphicRectItem(m_pixmapItem);
			pItem->setOpacity(0.5);

			pItem->setPos(obj["x"].toInt(), obj["y"].toInt());
			pItem->setRect(obj["x"].toInt(), obj["y"].toInt(), obj["width"].toInt(), obj["height"].toInt());
			pItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

			ItemManagerInstance()->addFence(name, pItem);
			scene->addItem(pItem);
		}
		else
			pItem = ItemManagerInstance()->getFence(name);

		if (pItem)
		{
			pItem->setBaseInfo(name, type, color);
		}
	}
}

void DraughtSoft::unCheckOhterButton(bool check, QPushButton* me)
{
	if (!check)
		return;
	for (QPushButton* pBtn : findChildren<QPushButton*>())
	{
		if (pBtn == me)
			continue;
		if (!pBtn->isCheckable())
			continue;
		pBtn->setChecked(false);
	}
}
