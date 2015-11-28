#include "CommandProvider.h"
#include "qshell.h"
#include "QtCore\qmimedata.h"
#include "QShellUtils.h"
#include "QtWidgets\qfiledialog.h"
#include "QShellSyntaxHighlighter.h"
#include <utility>
#include "HistoryKeyHandler.h"
#include "MiscKeyHandler.h"
#include "ScriptEvaluatorKeyHandler.h"
#include "CompleterKeyHandler.h"
#include "DefaultKeyHandler.h"
#include "ClearConsole.h"

using namespace std;


CommandProvider::CommandProvider(QShell& pShell)
	: shell(pShell), menuBuilder(pShell)
{
	connect(&menuBuilder, SIGNAL(ClearActionTriggered()), this, SLOT(onClearAction()));
	connect(&menuBuilder, SIGNAL(LoadScriptActionTriggered()), this, SLOT(onLoadScriptAction()));

	new QShellSyntaxHighlighter(shell.document()); // probably to move	
}

void CommandProvider::SetHandlers( HandlersVec h )
{
	handlers = move(h);
}

void CommandProvider::ChangeFontSize(int points) 
{
	auto currentFont = shell.font();
	currentFont.setPointSize(currentFont.pointSize()+points);
	shell.setFont(currentFont);
}

void CommandProvider::onKeyPress( QKeyEvent* key )
{
	shell.setReadOnly(false);
	auto it = find_if(begin(handlers), end(handlers), 
		[key, this](IKeyPressHandler* handler){
		return handler->onKeyPressed(shell, key);
	});
}

void CommandProvider::onMouseWheelEvent( QWheelEvent* e)
{
	if (false == e->modifiers().testFlag(Qt::ControlModifier))
		return shell.processWheelEvent(e);

	auto points = e->delta()>0 ? 1 : -1;
	ChangeFontSize(points);
	AcceptEvent(shell, e);
}

void CommandProvider::onContextMenuEvent( QContextMenuEvent *event )
{
	shell.setReadOnly(!CursorCanWrite(shell));
	auto contextMenu = menuBuilder.CreateMenu();
	contextMenu->exec(event->globalPos());
}

void CommandProvider::onInsertFromMimeData( const QMimeData * source )
{
	auto&& text = source->text();
	auto tokens = text.split("\n");

	if (tokens.empty())
		return;

	if (false == CursorCanWrite(shell)) 
		MoveCursorAtEnd(shell);	

	shell.insertPlainText(tokens[0]);
	for(auto i=1; i<tokens.length(); ++i) 
	{
		shell.insertPlainText(QChar(8232)); // maybe windows dependent
		shell.insertPlainText(tokens[i]);
	}
}

// *************************************************** Context Menu actions
void CommandProvider::onClearAction()
{
	shell.clear();
	shell.setPlainText(shell.Prompt());
	MoveCursorAtEnd(shell);
}

void CommandProvider::onLoadScriptAction()
{
	QFileDialog fileDialog;
	MoveCursorAtEnd(shell);
	QString file = fileDialog.getOpenFileName(&shell,"Load Script","","Javascript files (*.js);;Text files (*.txt);;All files (*.*)");

	if (false == file.isNull()) 
	{
		emit ScriptLoadRequested(file);
	}
}

void CommandProvider::SetClc(ClearConsole& clc)
{
	connect(&clc, &ClearConsole::ClcCalled, [this]{
		shell.clear();
	});
}

// *************************************************** Context Menu actions
