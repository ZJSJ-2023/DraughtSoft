#include "customwebsocket.h"

#include <QGraphicsScene>
#include "customgraphicpixmapitem.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QtWebSockets/QWebSocket>
#include "httpfunc.h"
#include <QTimer>
#include <qobject.h>
#include <QMessageBox>
#include "globalinstance.h"
#include "global.h"

CustomWebSocket::CustomWebSocket(QGraphicsScene* scene, QGraphicsPixmapItem* parentItem)
	: m_scene(scene)
	, m_parentItem(parentItem)
{

}

void CustomWebSocket::startToConnect()
{
	login();
}

void CustomWebSocket::login()
{
	QString loginUrl = QString("http://%0/paas/api/user/login?loginCode=%1&loginPwd=%2").arg(SERVR_ADDR).arg("chongq").arg("chongq");
	QJsonObject root;
	root.insert("loginCode", "chongq");
	root.insert("loginPwd", "chongq");

	HttpFunc::post(loginUrl, QJsonDocument(root).toJson(), [=](int error, const QString& str) {
		if (error != 0)
		{
			qDebug() << "login failed!";
			return;
		}

		getMapId(str);
	});
}

void CustomWebSocket::getMapId(const QString& json)
{
	QJsonObject root = QJsonDocument::fromJson(json.toUtf8().data()).object();

	token = root["entity"].toObject()["api_token"].toString();

	// 获取地图列表
	QString mapUrl = QString("http://%0/paas/api/super/map/list?").arg(SERVR_ADDR);
	mapUrl.append(QString("api_token=%0").arg(token));
	HttpFunc::request(mapUrl, [=](int error, const QString& str) {
		if (error != 0)
		{
			qDebug() << "get map id failed!";
			return;
		}

		getTagList(str);
	});
}

void CustomWebSocket::getTagList(const QString& json)
{
	// json中包含mapId，但是这里固定为1

	//获取标签列表
	QString listUrl = QString("http://%0/paas/api/project/tag/list?").arg(SERVR_ADDR);
	listUrl.append(QString("sort=%0").arg("id"));
	listUrl.append(QString("&order=%0").arg("desc"));
	listUrl.append(QString("&offset=%0").arg(0));
	listUrl.append(QString("&limit=%0").arg(5));
	listUrl.append(QString("&status=%0").arg("offline"));
	listUrl.append(QString("&mapId=%0").arg(1));
	listUrl.append(QString("&fields=%0").arg("[\"mapId\",\"code\",\"id\"]"));
	listUrl.append(QString("&api_token=%0").arg(token));

	HttpFunc::request(listUrl, [=](int error, const QString& str) {
		if (error != 0)
		{
			qDebug() << "get tag list failed!";
			return;
		}

		connectWebSocket(str);
	});
}

void CustomWebSocket::connectWebSocket(const QString& json)
{
	// json中包含了tag编码，需要根据这些编码来获取tag实时位置

	QWebSocket* socket = new QWebSocket();
	// todo: need code to replace 100000
	QString webstr = QString("ws://%0/websocket/tag_%1_2D%2_type|%3").arg(SERVR_ADDR).arg(100000).arg(token).arg("coord");

	QObject::connect(socket, &QWebSocket::connected, [=]() {
		qDebug() << "connect to the web server";
	});

	QObject::connect(socket, &QWebSocket::disconnected, [=]() {
		QMessageBox::warning(MainWindowInstance(), QObject::tr("Warning"), QObject::tr("websocket has disconnected!"), QMessageBox::Ok);
	});

	QObject::connect(socket, &QWebSocket::textMessageReceived, [=](const QString& message) {
		qDebug() << message;
	});

	socket->open(QUrl(webstr));

	int error = socket->error();

	int a = 10;
}
