#pragma once
#include <qwidget.h>
#include <QDialog>
class SubInfoWidget : public QDialog
{
	Q_OBJECT
public:
	SubInfoWidget(QWidget* parent = nullptr);
	~SubInfoWidget(){}

	void setOnline(bool b);
	void setId(int id);
	void setSpeed(int speed);

private:
	int getId() const;
	int getSpeed() const;

signals:
	void sigLock(bool lock);
};

