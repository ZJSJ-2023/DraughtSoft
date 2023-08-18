#pragma once
#include <QFont>

namespace Appsettings
{
	QFont getAppFont(int pointsize = 10);
	QString getAppDataPath();
	QString getImgPath();
};

