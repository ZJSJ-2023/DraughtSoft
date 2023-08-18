#pragma once
#include <qgraphicsview.h>
class CustomGraphicView : public QGraphicsView
{
	Q_OBJECT
public:
	CustomGraphicView(QWidget* parent = nullptr);
	~CustomGraphicView(){}

public:
	void zoomIn();
	void zoomOut();
	void zoomReset();

private:
	qreal  m_zoomInValue;
	qreal  m_zoomOutValue;
};

