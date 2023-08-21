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
	m_pixmapItem->setFlags(QGraphicsItem::ItemIsMovable);
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
		unCheckOhterButton(check,pCreateFenceBtn);

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

		m_pixmapItem->setFlags(check ? (m_pixmapItem->flags() & ~QGraphicsItem::ItemIsMovable)
							   : (m_pixmapItem->flags() | QGraphicsItem::ItemIsMovable));
	});

	setCentralWidget(m_pGraphicView);

	//updateItems("");

	// 第一次请求，初始化数据
	HttpFunc::request("http://localhost:8080/allInfo", std::bind(&DraughtSoft::updateItems, this, std::placeholders::_1));

	QTimer* pTimer = new QTimer(this);
	pTimer->setInterval(5000);
	pTimer->start();
	connect(pTimer, &QTimer::timeout, this, [=]() {
		HttpFunc::request("http://localhost:8080/allInfo", std::bind(&DraughtSoft::updateItems, this, std::placeholders::_1));
	});
}

//#define TEST
void DraughtSoft::updateItems(QString str)
{
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

	// 解析
	{
		QJsonObject root = QJsonDocument::fromJson(jsonStr.toUtf8().data()).object();

		QJsonArray array = root["FFU Array"].toArray();

		for (int i = 0; i < array.size(); ++i)
		{
			QJsonObject obj = array[i].toObject();
			int id = obj["id"].toInt();

			// 如果已经存在，那么只是刷新信息，如果不存在，那么加入进去
			if (!idToItemMap.contains(id))
			{
				CustomGraphicPixmapItem* pItem = new CustomGraphicPixmapItem(m_pixmapItem);
				pItem->setId(id);
				pItem->setOnline(obj["state"].toInt());
				pItem->setSpeed(obj["speed"].toInt());

#ifdef TEST
				QRectF rect = pItem->boundingRect();
				int x = i % 5;
				int y = i / 5;
				pItem->setPos(x * rect.width() * 1.2, y * rect.height() * 1.2);
#else if
				pItem->setPos(obj["x"].toInt(), obj["y"].toInt());
#endif
				pItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

				idToItemMap[id] = pItem;
				scene->addItem(pItem);
			}
			else
			{
				CustomGraphicPixmapItem* pItem = idToItemMap[id];
				pItem->setOnline(obj["state"].toInt());
				pItem->setSpeed(obj["speed"].toInt());
				pItem->updatePixmap();
			}
		}

		scene->update();
	}
}

// post pos information to server
void DraughtSoft::postPos()
{
	QJsonObject root;

	QJsonArray array;
	for (QGraphicsItem* pItem : m_pixmapItem->childItems())
	{
		CustomGraphicPixmapItem* pCustomItem = dynamic_cast<CustomGraphicPixmapItem*>(pItem);
		QJsonObject ffu;
		ffu.insert("id", pCustomItem->getId());
		ffu.insert("x", pCustomItem->pos().x());
		ffu.insert("y", pCustomItem->pos().y());

		array.push_back(ffu);
	}

	root.insert("FFU Array", array);

	const QString jsonStr = QJsonDocument(root).toJson();

	HttpFunc::post("", jsonStr, [=](QString)->void {
		// do something 
	});
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
