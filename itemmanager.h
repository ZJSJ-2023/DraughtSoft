#pragma once
#include <QMap>

class CustomGraphicPixmapItem;
class CustomGraphicRectItem;
class ItemManager
{
public:
	ItemManager();
	~ItemManager() {}

	bool addFFu(int id, CustomGraphicPixmapItem* FFu);
	bool containsFFu(int id);
	CustomGraphicPixmapItem* getFFu(int id);

	bool addFence(const QString& name, CustomGraphicRectItem* Fence);
	bool containseFence(const QString& name);
	CustomGraphicRectItem* getFence(const QString& name);

private:
	QMap<int, CustomGraphicPixmapItem*> idToFFuMap;
	QMap<QString, CustomGraphicRectItem*> nameToFenceMap;
};

