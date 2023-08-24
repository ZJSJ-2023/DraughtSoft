#pragma once

#include <QString>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class CustomWebSocket
{
public:
	CustomWebSocket(QGraphicsScene* scene, QGraphicsPixmapItem* parentItem);
	~CustomWebSocket(){}

	void startToConnect();

private:
	void login();
	void getMapId(const QString& json);
	void getTagList(const QString& json);
	void connectWebSocket(const QString& json);


private:
	QGraphicsScene* m_scene;
	QGraphicsPixmapItem* m_parentItem;

	QString token;
};

