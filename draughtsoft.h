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

private:
	void updateItems(QString str);

	void postPos();

private:
	Ui::DraughtSoftClass ui;
	QPointer<CustomGraphicView>  m_pGraphicView;
	CustomGraphicScene* scene = nullptr;
	QMap<int, CustomGraphicPixmapItem*> idToItemMap;
	QGraphicsPixmapItem* m_pixmapItem;
};
