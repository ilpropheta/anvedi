#include "HistoryKeyHandler.h"
#include "QShellUtils.h"
#include "QShellEngineResult.h"


bool HistoryKeyHandler::onKeyPressed( QShell& shell, QKeyEvent* e )
{
	auto key = e->key();
	if ( (key == Qt::Key_Up || key == Qt::Key_Down) && !IsMultiLine(shell))
	{	
		DeleteLastBlockContentKeepingPromptString(shell);
		auto historyFn = (key == Qt::Key_Up) ? &HistoryRecorder<QString>::Next : &HistoryRecorder<QString>::Previous;
		shell.insertPlainText( (history.*historyFn)() );
		AcceptEvent(shell, e);
		return true;
	}
	return false;
}

void HistoryKeyHandler::onTextEvaluated( const QString& text, QShellEngineResult& )
{
	history.Add(text);
}
