#pragma once
#include <qgraphicsscene.h>
#include <QGraphicsSceneMouseEvent>
#include <QPointer>
#include <QPainter>
#include <QGraphicsRectItem>

class CustomGraphicScene : public QGraphicsScene
{
	Q_OBJECT
public:
	enum ENUM_DrawingGraphic {
		Drawing_Normal,
		Drawing_Circular,
		Drawing_StraightLine,
		Drawing_Rectangular,
		Drawing_Triangle,
		Drawing_ManyLineSegements,
		Drawing_Curve
	};

public:
	CustomGraphicScene(QObject* parent = nullptr);
	CustomGraphicScene() {}

	void setCreateFence(bool b) { createFence = b; }

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent* e) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* e) override;

private:
	void DrawRealShape(std::vector<QPointF> vetPt);

private:
	QPoint  topLeft;
	QPoint  bottomRight;

	bool    startDraw = false;
	bool    createFence = false;

	QGraphicsRectItem* pCurrentItem = nullptr;


};

