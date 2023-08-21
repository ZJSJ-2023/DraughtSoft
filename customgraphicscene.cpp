#include "customgraphicscene.h"
#include <QPainter>
#include "createfencedlg.h"
#include "customgraphicrectitem.h"
#include "globalinstance.h"

CustomGraphicScene::CustomGraphicScene(QObject* parent /*= nullptr*/)
	: QGraphicsScene(parent)
{
	pCurrentItem = new QGraphicsRectItem();
}

#include <QDebug>
void CustomGraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
	QGraphicsScene::mouseMoveEvent(e);

	if (!createFence)
		return;

	if (startDraw)
	{
		QRect rect;
		rect.setTopLeft(topLeft);
		rect.setBottomRight(e->scenePos().toPoint());

		pCurrentItem->setRect(rect);
		pCurrentItem->update();
		update();
	}
}

void CustomGraphicScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mousePressEvent(event);

	if (!createFence)
		return;

	if (!startDraw)  // 第一个点未选，选择第一个点
	{
		QPointF f = event->scenePos();
		topLeft = event->scenePos().toPoint();
		startDraw = true;
		addItem(pCurrentItem);
	}
}

void CustomGraphicScene::DrawRealShape(std::vector<QPointF> vetPt)
{
	QRectF rect;
	rect.setTopLeft(RootItemInstance()->mapFromScene(vetPt[0]).toPoint());
	rect.setBottomRight(RootItemInstance()->mapFromScene(vetPt[1]).toPoint());

	CustomGraphicRectItem* pItem = new CustomGraphicRectItem(RootItemInstance());
	pItem->setRect(rect);
	pItem->setOpacity(0.5);
	addItem(pItem);

	CreateFenceDlg dlg;
	if (dlg.exec())
	{
		QString name = dlg.getName();
		QString type = dlg.getType();
		int tag = dlg.getTag();

		Qt::GlobalColor color;
		switch (tag)
		{
		case 0:
			color = Qt::green;
			break;
		case 1:
			color = Qt::cyan;
			break;
		case 2:
			color = Qt::yellow;
			break;
		case 3:
			color = Qt::blue;
			break;
		default:
			break;
		}

		pItem->setBaseInfo(name, type, tag);
		pItem->setBrush(color);
	}
	else
	{
		removeItem(pItem);
	}

}

void CustomGraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	QGraphicsScene::mouseReleaseEvent(e);

	if (!createFence)
		return;

	if (startDraw) // 第一个点已选，开始选第二个点
	{
		bottomRight = e->scenePos().toPoint();
		//选完第二个点，开始画矩形
		DrawRealShape({ topLeft,bottomRight });

		removeItem(pCurrentItem);

		startDraw = false;
	}
}




