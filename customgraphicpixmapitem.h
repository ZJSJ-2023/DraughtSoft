#pragma once
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QPixmap>

class SubInfoWidget;
class CustomGraphicPixmapItem : public QGraphicsPixmapItem
{
public:
	CustomGraphicPixmapItem(QGraphicsItem* parent = nullptr);
	~CustomGraphicPixmapItem() {}

	void setId(int id);
	int getId() const { return m_id; }

	void setOnline(bool b) { m_onLine = b; }
	void setSpeed(int speed) { m_speed = speed; }
	void update();

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
	QPixmap* createPixmap(int id, int speed = -1);

private:
	bool showInfo = false;
	SubInfoWidget* pTipWidget = nullptr;
	QPixmap* m_pixmap = nullptr;

	int  m_id;
	bool m_onLine;
	int  m_speed;
};

