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

	if(!startDraw)  // ��һ����δѡ��ѡ���һ����
	{
		topLeft = event->scenePos().toPoint();
		startDraw = true;
		addItem(pCurrentItem);
	}
}

void CustomGraphicScene::DrawRealShape(std::vector<QPointF> vetPt)
{
	QRectF rect;
	rect.setTopLeft(vetPt[0]);
	rect.setBottomRight(vetPt[1]);

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

	if (startDraw) // ��һ������ѡ����ʼѡ�ڶ�����
	{
		bottomRight = e->scenePos().toPoint();
		//ѡ��ڶ����㣬��ʼ������
		DrawRealShape({ topLeft,bottomRight });

		removeItem(pCurrentItem);

		startDraw = false;
	}
}




