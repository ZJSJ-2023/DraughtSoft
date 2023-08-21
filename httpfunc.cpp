#include "httpfunc.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

namespace HttpFunc
{
	void request(QString url, std::function<void(QString)> callback)
	{
		//http请求
		QNetworkRequest request;
		QNetworkAccessManager* nam = new QNetworkAccessManager();
		QObject::connect(nam, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
			callback(reply->readAll());
		});

		request.setUrl(QUrl(url));

		//请求头token设置
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
		QString token = "";
		QString token_headerData = token;
		request.setRawHeader("token", token_headerData.toLocal8Bit());

		nam->get(request);
	}

	void post(QString url, QString json, std::function<void(QString)> callback)
	{
		//http请求
		QNetworkRequest request;
		QNetworkAccessManager* nam = new QNetworkAccessManager();
		QObject::connect(nam, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
			callback(reply->readAll());
		});

		request.setUrl(QUrl(url));

		//请求头token设置
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
		QString token = "";
		QString token_headerData = token;
		request.setRawHeader("token", token_headerData.toLocal8Bit());

		QNetworkReply* reply = nam->post(request, json.toLocal8Bit());
	}


};