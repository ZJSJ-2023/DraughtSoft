#include "globalinstance.h"
#include "itemmanager.h"

GlobalInstance::GlobalInstance()
{

}

ItemManager* GlobalInstance::getItemManager()
{
	if (!m_pItemManager)
		m_pItemManager = new ItemManager();

	return m_pItemManager;
}




