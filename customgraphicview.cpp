#include "customgraphicview.h"

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
