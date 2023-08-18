#pragma once
#include <qglobal.h>
#include <QMainWindow>


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

private:
	explicit GlobalInstance();
	Q_DISABLE_COPY_MOVE(GlobalInstance);

private:
	QMainWindow* m_pMainWindow;
};

namespace
{
	inline static QMainWindow* MainWindowInstance() { return GlobalInstance::getInstance().getMainWindow(); }
}

