#include "QShellContextMenuBuilder.h"
#include "qshell.h"
#include "QtCore\qobject.h"

QShellContextMenuBuilder::QShellContextMenuBuilder(QShell& pShell)
	: shell(pShell)
{
		// Clear Action
	m_ClearAction = new QAction(tr("Clear"), &shell);
	m_ClearAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
	QObject::connect(m_ClearAction, SIGNAL(triggered()), this, SIGNAL(ClearActionTriggered()));
	shell.addAction(m_ClearAction);

	// LoadScript Action
	m_LoadScriptAction = new QAction(tr("Load Script"), &shell);
	m_LoadScriptAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
	QObject::connect(m_LoadScriptAction, SIGNAL(triggered()), this, SIGNAL(LoadScriptActionTriggered()));
	shell.addAction(m_LoadScriptAction);
}

std::unique_ptr<QMenu> QShellContextMenuBuilder::CreateMenu()
{
	std::unique_ptr<QMenu> menu (shell.createStandardContextMenu());
	menu->addAction(m_ClearAction);
	menu->addAction(m_LoadScriptAction);
	return menu;
}
