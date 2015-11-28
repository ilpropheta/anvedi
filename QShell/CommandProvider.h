#pragma once

#include <QObject>
#include "QtGui\qevent.h"
#include "QShellContextMenuBuilder.h"
#include "KeyPressHandler.h"

class QShell;
class ClearConsole;

typedef std::vector<IKeyPressHandler*> HandlersVec;

class CommandProvider : public QObject
{
	Q_OBJECT
public:
	CommandProvider(QShell& pShell);

	void onKeyPress(QKeyEvent* key);
	void onMouseWheelEvent(QWheelEvent* key);
	void onContextMenuEvent(QContextMenuEvent *event);
	void onInsertFromMimeData ( const QMimeData * source );
	void SetHandlers(HandlersVec h);
	void SetClc(ClearConsole& clc);
private slots:
	void onClearAction();
	void onLoadScriptAction();
signals:
	void ScriptLoadRequested(const QString&);
private:
	void ChangeFontSize(int points);

	QShell&						shell;
	QShellContextMenuBuilder	menuBuilder;
	HandlersVec					handlers;
};

