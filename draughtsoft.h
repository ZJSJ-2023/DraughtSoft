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

class DraughtSoft : public QMainWindow
{
	Q_OBJECT

public:
	DraughtSoft(QWidget* parent = Q_NULLPTR);

private:
	void unCheckOhterButton(bool check, QPushButton* me);

	void updateItems(QString jsonstr);

private:
	Ui::DraughtSoftClass ui;
	QPointer<CustomGraphicView>  m_pGraphicView;
	CustomGraphicScene* scene = nullptr;
	QMap<int, CustomGraphicPixmapItem*> idToItemMap;
};
