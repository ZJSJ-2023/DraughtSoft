#include "customgraphicpixmapitem.h"
#include "subinfowidget.h"
#include <QApplication>
#include <QCoreApplication>
#include "globalinstance.h"
#include "appsettings.h"

CustomGraphicPixmapItem::CustomGraphicPixmapItem(QGraphicsItem* parent /*= nullptr*/)
	:QGraphicsPixmapItem(parent)
{
	setAcceptHoverEvents(true);

	pTipWidget = new SubInfoWidget(MainWindowInstance());
	pTipWidget->hide();
}

void CustomGraphicPixmapItem::setId(int id)
{
	m_id = id;
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

QPixmap* CustomGraphicPixmapItem::createPixmap(int id, int speed)
{
	QPixmap* pixmap = new QPixmap(128, 72);
	pixmap->fill(Qt::transparent);
	QPainter painter(pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);

	QPen pen;
	pen.setColor(Qt::red);
	painter.setPen(pen);

	QFont font;
	font.setFamily("Arial");
	font.setPointSize(10);
	painter.setFont(font);

	//QPointF pos = this->pos();

	painter.drawText(0, 0, 32, 16, Qt::AlignLeft, QString("ID:%0").arg(id));
	//painter.drawText(0, 16, 64, 16, Qt::AlignLeft, QString("(%0, %1)").arg((int)pos.x()).arg((int)pos.y()));
	painter.drawText(0, 16, 128, 16, Qt::AlignLeft, QString("speed:%0").arg(speed));
	//painter.drawPixmap(QPoint(0, 40), QPixmap(Appsettings::getImgPath() + "location.png"));

	return pixmap;
}

void CustomGraphicPixmapItem::update()
{
	if (m_pixmap)
	{
		delete m_pixmap;
		m_pixmap = nullptr;
	}

	m_pixmap = createPixmap(m_id, m_speed);
	setPixmap(*m_pixmap);

	QGraphicsPixmapItem::update();
}
