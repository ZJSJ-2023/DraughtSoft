#pragma once
#include <QDialog>
#include <QString>
class CreateFenceDlg : public QDialog
{
	Q_OBJECT
public:
	CreateFenceDlg(QWidget* parent = nullptr);
	~CreateFenceDlg(){}

	QString getName() const;
	QString getType() const;

	int getTag() const;

signals:
	void sigOk();
	void sigCancel();
};

