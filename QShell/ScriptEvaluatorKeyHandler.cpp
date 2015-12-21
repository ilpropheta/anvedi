#include "ScriptEvaluatorKeyHandler.h"
#include "qevent.h"
#include "qshell.h"
#include "QShellUtils.h"
#include <QString>

ScriptEvaluatorKeyHandler::ScriptEvaluatorKeyHandler( QShell& s )
	: shell(s)
{

}

bool ScriptEvaluatorKeyHandler::onKeyPressed( QShell& shell, QKeyEvent* e )
{
	if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
	{
		auto textToEval = StringToEvaluate(shell);
		if (textToEval.isEmpty())
		{
			AcceptEvent(shell, e);
			return true;
		}

		MoveCursorAtEnd(shell);
		auto result = engine->Evaluate(textToEval);
		emit TextEvaluated(textToEval, *result);

		InsertEvaluationResult(shell, textToEval.endsWith(";") ? *engine->MakeEmptyResult() : *result);
		AcceptEvent(shell, e);
		return true;
	}

	return false;
}

void ScriptEvaluatorKeyHandler::onNewEngine( std::shared_ptr<QShellEngine> e )
{
	engine = e;
}

void ScriptEvaluatorKeyHandler::onLoadScript( const QString& file )
{
	// possible refactoring: handle this with a SIGNAL 
	// (handled by the CommandProvider or someone else)
	auto result = engine->LoadScript(file);
	InsertEvaluationResult(shell, *result);
	shell.ensureCursorVisible();
}
