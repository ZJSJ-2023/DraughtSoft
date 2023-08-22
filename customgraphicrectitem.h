#pragma once
#include <qgraphicsitem.h>
class CustomGraphicRectItem : public QGraphicsRectItem
{
public:
	CustomGraphicRectItem(QGraphicsItem* parent = nullptr);
	~CustomGraphicRectItem() {}

	void setBaseInfo(const QString& name, const QString& type, int color);

	QString getName() const { return m_name; }
	QString getType() const { return m_type; }
	int    getColor() const { return m_color; }

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
	void showTips(bool visible);

private:
	QString m_name;
	QString m_type;
	int  m_color;

	QWidget* pTipsWidget = nullptr;
};

