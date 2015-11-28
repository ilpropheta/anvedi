#include "CompleterKeyHandler.h"
#include "QShellCompleter.h"
#include "qshell.h"
#include "QShellUtils.h"
#include "QAbstractItemView.h"
#include "qstringlistmodel.h"
#include "qtwidgets\qscrollbar.h"

CompleterKeyHandler::CompleterKeyHandler( QShell& s )
	: shell(s)
{
	completer = new QShellCompleter(&shell); // potrei passargli direttamente il completer
	completer->setWidget(&shell);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setSeparator(QLatin1String("."));

	QObject::connect(completer, SIGNAL(activated(QString)),
		this, SLOT(onInsertSuggestion(const QString&)));
}

bool CompleterKeyHandler::onKeyPressed( QShell& shell, QKeyEvent* e )
{
	if (e->key() == Qt::Key_Tab)
	{
		MoveCursorAtEndIfBadPosition(shell);
		if (e->modifiers().testFlag(Qt::ShiftModifier))
		{
			shell.processKeyEvent(e);
			return true;
		}
		if (completer->popup()->isVisible())
		{
			completer->popup()->hide();
			IgnoreEvent(shell, e);
			return true;
		}
		ShowCompletion(shell);
		return true;
	}
	if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
	{
		if (completer->popup()->isVisible())
		{
			IgnoreEvent(shell, e);
			return true;
		}
	}
	if (e->key() == Qt::Key_Backspace)
	{
		if (CursorCanWrite(shell) && (shell.textCursor().hasSelection() || CursorBlockDifference(shell) > 0))
		{
			shell.processKeyEvent(e);
			if (completer->popup()->isVisible())
			{
				ShowCompletion(shell);
			}
		}
		AcceptEvent(shell, e);
		return true;
	}
	if (e->key() == Qt::Key_Escape)
	{
		if (completer->popup()->isVisible())
		{
			completer->popup()->hide();
			AcceptEvent(shell, e);
			return true;
		}
	}

	return false;
}

void CompleterKeyHandler::onUnknownKeyProcessed( QShell& shell, QKeyEvent* e )
{
	if (completer->popup()->isVisible())
		ShowCompletion(shell);
}

// not should be here
QAbstractItemModel* CompleterKeyHandler::intelliNaiveModel(const QString& rootObj)
{
	return new QStringListModel(engine->Suggest(rootObj), completer);
}

void CompleterKeyHandler::ShowCompletion(QShell& shell)
{
	auto completionPrefix = TextUnderCursor(shell);
	auto toSuggest = completionPrefix;
	if (completionPrefix.isEmpty())
		toSuggest = "this";
	else
	{
		auto lastDot = completionPrefix.lastIndexOf('.');
		if (lastDot>0)
		{
			toSuggest = completionPrefix.mid(0, lastDot);
			if (completionPrefix.endsWith('.'))
				completionPrefix = "";
			else completionPrefix = completionPrefix.mid(lastDot+1);
		}
		else
		{
			toSuggest = "this";
		}
	}

	completer->setModel(intelliNaiveModel(toSuggest));

	//
	if (completionPrefix != completer->completionPrefix()) {
		completer->setCompletionPrefix(completionPrefix);
		completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
	}
	QRect cr = shell.cursorRect();
	cr.setWidth(completer->popup()->sizeHintForColumn(0)
		+ completer->popup()->verticalScrollBar()->sizeHint().width());
	auto popup = completer->popup();
	completer->popup()->setVisible(true);
	completer->complete(cr); // popup it up!
}

void CompleterKeyHandler::onNewEngine( std::shared_ptr<QShellEngine> e)
{
	engine = e;
	completer->setModel(intelliNaiveModel("this"));
}

void CompleterKeyHandler::onInsertSuggestion( const QString& suggestion )
{
	// possible refactoring: handle this with a SIGNAL 
	// (handled by the CommandProvider or someone else)
	if (CursorCanWrite(shell) && completer)
	{
		auto tc = shell.textCursor();
		auto prefixLen = completer->completionPrefix().length();
		if (prefixLen > 0)
		{
			tc.selectionStart();
			tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, prefixLen);
			tc.selectionEnd();
			tc.removeSelectedText();
		}
		tc.insertText(suggestion);
		shell.setTextCursor(tc);
	}
}
