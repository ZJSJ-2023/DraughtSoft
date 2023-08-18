#include "customgraphicrectitem.h"
#include <QFormLayout>
#include "uifactory.h"
#include <QObject>
#include <QMap>

CustomGraphicRectItem::CustomGraphicRectItem(QGraphicsItem* parent /*= nullptr*/)
	:QGraphicsRectItem(parent)
{
	setAcceptHoverEvents(true);
}

void CustomGraphicRectItem::setBaseInfo(const QString& name, const QString& type, int color)
{
	m_name = name;
	m_type = type;
	m_color = color;
}

void CustomGraphicRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{

	showTips(true);
	QGraphicsRectItem::hoverEnterEvent(event);
}

void CustomGraphicRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	showTips(false);
	QGraphicsRectItem::hoverLeaveEvent(event);
}

void CustomGraphicRectItem::showTips(bool visible)
{
	if (!pTipsWidget)
	{
		pTipsWidget = new QWidget();
		pTipsWidget->setObjectName("tipWidget");
		pTipsWidget->setStyleSheet("QWidget#tipWidget{ background-color:rgba(128,138,135,0.3);color:whtie;border-radius:20px;}");
		pTipsWidget->setWindowFlags(Qt::FramelessWindowHint);
		QFormLayout* pLayout = new QFormLayout(pTipsWidget);

		ControlPorperty property1, property2;
		pLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(QObject::tr("Fence Name:")).setStylesheet("color:white;")),
						UiFactory::createControl<QLabel>(property2.init().setText(m_name).setReadOnlyState(true).setStylesheet("color:white;")));

		pLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(QObject::tr("Fence Type:")).setStylesheet("color:white;")),
						UiFactory::createControl<QLabel>(property2.init().setText(m_type).setReadOnlyState(true).setStylesheet("color:white;")));

		QMap<int, QString> map = {
								 {0,QObject::tr("Green Group")},
								 {1,QObject::tr("Orange Group")},
								 {2,QObject::tr("Yellow Group")},
								 {3,QObject::tr("Blue Group")} };
		pLayout->addRow(UiFactory::createControl<QLabel>(property1.init().setText(QObject::tr("Tag:")).setStylesheet("color:white;")),
						UiFactory::createControl<QLabel>(property2.init().setText(map[m_color]).setReadOnlyState(true).setStylesheet("color:white;")));
	}

	pTipsWidget->move(QCursor::pos());
	pTipsWidget->setVisible(visible);
}
