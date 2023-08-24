#pragma once

#include <QString>
#include <functional>

namespace HttpFunc
{
	void request(QString url, std::function<void(int,const QString&)> callback);
	void post(QString url, QString json, std::function<void(int,const QString&)> callback);
};

