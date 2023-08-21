#pragma once
#include <qglobal.h>
#include <QMainWindow>

#include <QGraphicsItem>

class GlobalInstance
{
public:
	~GlobalInstance() {}

	static GlobalInstance& getInstance()
	{
		static GlobalInstance history;
		return history;
	}

	void setMainWindow(QMainWindow* window) { m_pMainWindow = window; }
	QMainWindow* getMainWindow() { return m_pMainWindow; }

	void setRootItem(QGraphicsItem* pItem) { m_pRootItem = pItem; }
	QGraphicsItem* getRootItem() { return m_pRootItem; }

private:
	explicit GlobalInstance();
	Q_DISABLE_COPY_MOVE(GlobalInstance);

private:
	QMainWindow* m_pMainWindow;
	QGraphicsItem* m_pRootItem = nullptr;
};

namespace
{
	inline static QMainWindow* MainWindowInstance() { return GlobalInstance::getInstance().getMainWindow(); }
	inline static QGraphicsItem* RootItemInstance() { return GlobalInstance::getInstance().getRootItem(); }
}

