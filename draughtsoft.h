#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_draughtsoft.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointer>
#include <QPushButton>
#include <QMap>

class CustomGraphicView;
class CustomGraphicPixmapItem;
class CustomGraphicScene;
class QGraphicsPixmapItem;

class DraughtSoft : public QMainWindow
{
	Q_OBJECT

public:
	DraughtSoft(QWidget* parent = Q_NULLPTR);
	~DraughtSoft();

private:
	void updateItems(QString str);

	void postFFu();
	void postFences();

	void parseFFu(const QString& str);
	void parseFence(const QString& str);

	void unCheckOhterButton(bool check, QPushButton* me);
private:
	Ui::DraughtSoftClass ui;
	QPointer<CustomGraphicView>  m_pGraphicView;
	CustomGraphicScene* scene = nullptr;
	QGraphicsPixmapItem* m_pixmapItem;
};
