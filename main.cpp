#include "draughtsoft.h"
#include <QtWidgets/QApplication>


#include <QFormLayout>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include "customproxy.h"
#include "uifactory.h"
#include <QLabel>
#include "subinfowidget.h"

int main(int argc, char* argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

	QApplication a(argc, argv);
	DraughtSoft w;
	w.show();

	return a.exec();
}
