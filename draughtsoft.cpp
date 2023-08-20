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

	GlobalInstance::getInstance().setMainWindow(this);

	QToolBar* toolBar = new QToolBar(this);
	addToolBar(Qt::LeftToolBarArea, toolBar);
	toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	toolBar->setMovable(false);
	toolBar->setFloatable(false);

	ControlPorperty property;
	QPushButton* pZoomInBtn = UiFactory::createControl<QPushButton>(property.init().setText(tr("Zoom In")), this);
	QPushButton* pZoomOutBtn = UiFactory::createControl<QPushButton>(property.init().setText(tr("Zoom Out")), this);
	QPushButton* pCreateFenceBtn = UiFactory::createControl<QPushButton>(property.init().setCheckableState(true).setText(tr("Create Fence")), this);
	QPushButton* pMoveCloudBtn = UiFactory::createControl<QPushButton>(property.init().setCheckableState(true).setText(tr("Move Cloud")), this);

	toolBar->addWidget(pZoomInBtn);
	toolBar->addWidget(pZoomOutBtn);
	toolBar->addWidget(pCreateFenceBtn);
	toolBar->addWidget(pMoveCloudBtn);

	scene = new CustomGraphicScene(this);
	scene->setStickyFocus(true);

	QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();
	pixmapItem->setPixmap(QPixmap(Appsettings::getImgPath() + "map.png"));
	//pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	pixmapItem->setPos(0, 0);
	scene->addItem(pixmapItem);

	scene->setSceneRect(scene->itemsBoundingRect());

	m_pGraphicView = new CustomGraphicView(this);
	//m_pGraphicView->setDragMode(QGraphicsView::ScrollHandDrag);
	m_pGraphicView->setScene(scene);
	m_pGraphicView->scale(1, 1);
	m_pGraphicView->setRenderHints(m_pGraphicView->renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	m_pGraphicView->setAutoFillBackground(true);
	m_pGraphicView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	m_pGraphicView->move(0, 0);
	m_pGraphicView->show();

	connect(pZoomInBtn, &QPushButton::clicked, this, [=](bool check) {
		m_pGraphicView->zoomIn();
	});

	connect(pZoomOutBtn, &QPushButton::clicked, this, [=](bool check) {
		m_pGraphicView->zoomOut();
	});

	connect(pCreateFenceBtn, &QPushButton::toggled, this, [=](bool check) {
		unCheckOhterButton(check, pCreateFenceBtn);

		scene->setCreateFence(check);
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

void DraughtSoft::updateItems(QString jsonStr)
{
	// just for test
	/*QJsonObject root;
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

	root.insert("FFU array", array);

	const QString jsonStr = QJsonDocument(root).toJson();*/

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
				CustomGraphicPixmapItem* pItem = new CustomGraphicPixmapItem(this);
				pItem->setId(id);
				pItem->setOnline(obj["state"].toInt());
				pItem->setSpeed(obj["speed"].toInt());

				/*QRectF rect = pItem->boundingRect();
				int x = i % 5;
				int y = i / 5;*/

				//pItem->setPos(x * rect.width() * 1.2, y * rect.height() * 1.2);
				pItem->setPos(obj["x"].toInt(), obj["y"].toInt());
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
