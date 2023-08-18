#include "appsettings.h"
#include <QStandardPaths.h>
#include <QDir.h>
#include <QCoreApplication>

namespace Appsettings
{
	QFont getAppFont(int pointsize)
	{
		QFont font;
		font.setFamily("ÐÂËÎÌå,SimSun");
		font.setBold(false);
		font.setPointSize(pointsize);
		font.setStyleStrategy(QFont::PreferAntialias);

		return font;
	}

	QString getAppDataPath()
	{
		QString path = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

		QDir dir(path);
		if (!dir.exists())
			dir.mkpath(path);

		return path;
	}

	QString getImgPath()
	{
		return QCoreApplication::applicationDirPath() + "/img/";
	}
}
