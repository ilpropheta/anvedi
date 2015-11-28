#pragma once
#include "QtWidgets\qmenu.h"
#include <memory>

class QShell;

class QShellContextMenuBuilder : public QObject
{
	Q_OBJECT
public:
	QShellContextMenuBuilder(QShell& shell);

	std::unique_ptr<QMenu> CreateMenu();

signals:
	void ClearActionTriggered();	
	void LoadScriptActionTriggered();
private:
	QAction* m_ClearAction;
	QAction* m_LoadScriptAction;
	QShell& shell;
};

