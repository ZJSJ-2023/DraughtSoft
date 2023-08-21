#include "customgraphicview.h"
#include <QWheelEvent>

CustomGraphicView::CustomGraphicView(QWidget* parent /*= nullptr*/)
	:QGraphicsView(parent)
{
	m_zoomInValue = 1.1;
	m_zoomOutValue = 0.9;
}

void CustomGraphicView::zoomIn()
{
	/*m_zoomInValue -= 0.1;
	m_zoomOutValue -= 0.1;*/

	scale(m_zoomOutValue, m_zoomOutValue);
}

void CustomGraphicView::zoomOut()
{/*
	m_zoomInValue += 0.1;
	m_zoomOutValue += 0.1;*/

	scale(m_zoomInValue, m_zoomInValue);
}

void CustomGraphicView::zoomReset()
{

}

void CustomGraphicView::wheelEvent(QWheelEvent* e)
{
	if (e->modifiers() & Qt::ControlModifier) {
		if (e->angleDelta().y() > 0)
			zoomIn();
		else
			zoomOut();
		e->accept();
	}
	else {
		QGraphicsView::wheelEvent(e);
	}
}
