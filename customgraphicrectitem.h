#pragma once
#include <qgraphicsitem.h>
class CustomGraphicRectItem : public QGraphicsRectItem
{
public:
	CustomGraphicRectItem(QGraphicsItem* parent = nullptr);
	~CustomGraphicRectItem(){}

	void setBaseInfo(const QString& name, const QString& type, int color);

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

