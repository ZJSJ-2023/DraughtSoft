#include "itemmanager.h"

#include "customgraphicpixmapitem.h"
#include "customgraphicrectitem.h"

ItemManager::ItemManager()
{

}

bool ItemManager::addFFu(int id, CustomGraphicPixmapItem* FFu)
{
	if (idToFFuMap.contains(id))
		return false;

	idToFFuMap[id] = FFu;

	return true;
}

bool ItemManager::containsFFu(int id)
{
	return idToFFuMap.contains(id);
}

CustomGraphicPixmapItem* ItemManager::getFFu(int id)
{
	return idToFFuMap[id];
}

bool ItemManager::addFence(const QString& name, CustomGraphicRectItem* Fence)
{
	if (nameToFenceMap.contains(name))
		return false;

	nameToFenceMap[name] = Fence;

	return true;
}

bool ItemManager::containseFence(const QString& name)
{
	return nameToFenceMap.contains(name);
}

CustomGraphicRectItem* ItemManager::getFence(const QString& name)
{
	return nameToFenceMap[name];
}
