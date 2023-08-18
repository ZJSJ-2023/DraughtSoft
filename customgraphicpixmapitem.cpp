#include "customgraphicpixmapitem.h"
#include "subinfowidget.h"
#include <QApplication>
#include <QCoreApplication>
#include "globalinstance.h"
#include "appsettings.h"

CustomGraphicPixmapItem::CustomGraphicPixmapItem(QWidget* view, QGraphicsItem* parent /*= nullptr*/)
	:QGraphicsPixmapItem(parent)
{
	//setPixmap(QPixmap("D:/workplace/Github/DraughtSoft/x64/Release/location.png"));
	//setFlags(ItemIsMovable);
	setAcceptHoverEvents(true);

	pTipWidget = new SubInfoWidget();
	pTipWidget->hide();
}

void CustomGraphicPixmapItem::setId(int id)
{
	m_id = id;
	setPixmap(*createPixmap(m_id));
}

void CustomGraphicPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsPixmapItem::mouseDoubleClickEvent(event);

	showInfo = !showInfo;

	if (!pTipWidget->isVisible())
	{
		pTipWidget->setId(m_id);
		pTipWidget->setOnline(m_onLine);
		pTipWidget->setSpeed(m_speed);

		pTipWidget->move(QCursor::pos());
		pTipWidget->show();
	}
	else
	{
		//pTipWidget->hide();
	}
}

void CustomGraphicPixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	QGraphicsPixmapItem::hoverEnterEvent(event);
	setActive(true);

}

void CustomGraphicPixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	QGraphicsPixmapItem::hoverEnterEvent(event);
	setActive(false);

}

QPixmap* CustomGraphicPixmapItem::createPixmap(int id)
{
	QPixmap* pixmap = new QPixmap(32,48);
	pixmap->fill(Qt::transparent);
	QPainter painter(pixmap);

	QPen pen;
	pen.setColor(Qt::red);
	painter.setPen(pen);

	QFont font;
	font.setFamily("Arial");
	font.setPointSize(10);
	painter.setFont(font);

	painter.drawText(0,0,32,16,Qt::AlignLeft, QString("ID:%0").arg(id));
	painter.drawPixmap(QPoint(0, 16), QPixmap(Appsettings::getImgPath() + "location.png"));

	return pixmap;
}
