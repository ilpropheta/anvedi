#include "MiscKeyHandler.h"
#include "qevent.h"
#include "qshell.h"
#include "QShellUtils.h"
#include <QTextBlock>
#include <vector>
#include <functional>
#include <map>

using namespace std;

typedef map<int, function<bool(QShell&, QKeyEvent*)>> CommandMap;

#define REGISTER_FUNCTION(key, code)\
	fns.insert(make_pair(key, code))

CommandMap CreateCursorIndependantFunctions()
{
	CommandMap fns;
	REGISTER_FUNCTION(Qt::Key_Escape, [](QShell& shell, QKeyEvent* e){
		DeleteLastBlockContentKeepingPromptString(shell);
		return true;
	});
	REGISTER_FUNCTION(Qt::Key_Home, [](QShell& shell, QKeyEvent* e){
		auto currentCursor = shell.textCursor();
		auto moveMode = e->modifiers().testFlag(Qt::ShiftModifier) ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor;
		currentCursor.setPosition(shell.document()->lastBlock().position() + shell.Prompt().size(), moveMode);
		shell.setTextCursor(currentCursor);
		return true;
	});
	REGISTER_FUNCTION(Qt::Key_PageUp, [](QShell& shell, QKeyEvent* e){
		auto moveMode = e->modifiers().testFlag(Qt::ShiftModifier) ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor;
		shell.moveCursor(QTextCursor::Up, moveMode);
		return true;
	});
	REGISTER_FUNCTION(Qt::Key_PageDown, [](QShell& shell, QKeyEvent* e){
		auto moveMode = e->modifiers().testFlag(Qt::ShiftModifier) ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor;
		shell.moveCursor(QTextCursor::Down, moveMode);
		return true;
	});
	REGISTER_FUNCTION(Qt::Key_Escape, [](QShell& shell, QKeyEvent* e){
		DeleteLastBlockContentKeepingPromptString(shell);
		return true;
	});
	
	return fns;
}

vector<int> SpecialKeys() 
{
	vector<int> vect(8); 
	vect.push_back(Qt::Key_Shift); vect.push_back(Qt::Key_Control); 
	vect.push_back(Qt::Key_Alt);vect.push_back(Qt::Key_Left); 
	vect.push_back(Qt::Key_Right); vect.push_back(Qt::Key_End);
	vect.push_back(Qt::Key_Up); vect.push_back(Qt::Key_Down);
	return vect;
}

bool MiscKeyHandler::onKeyPressed( QShell& shell, QKeyEvent* e )
{
	static vector<int> specKeys = SpecialKeys();
	static CommandMap fns = CreateCursorIndependantFunctions();
	auto key = e->key();
	
	if (e->matches(QKeySequence::Copy) || e->matches(QKeySequence::Undo) || 
		e->matches(QKeySequence::Redo) || find(begin(specKeys), end(specKeys), key) != end(specKeys))
	{
		shell.processKeyEvent(e);
		return true;
	}

	auto it = fns.find(key);
	if (it != end(fns))
	{
		auto res = it->second(shell, e);
		AcceptEvent(shell, e);
		return res;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

CommandMap CreateCursorDependantFunctions()
{
	CommandMap fns;
	REGISTER_FUNCTION(Qt::Key_Backspace, [](QShell& shell, QKeyEvent* e){
		if (CursorCanWrite(shell) && (shell.textCursor().hasSelection() || CursorBlockDifference(shell) > 0))
		{
			shell.processKeyEvent(e);
		}
		return true;
	});
	REGISTER_FUNCTION(Qt::Key_Enter, [](QShell& shell, QKeyEvent* e){
		if (e->modifiers().testFlag(Qt::ShiftModifier))
		{
			shell.processKeyEvent(e);
			return true;
		}
		return false;
	});
	fns.insert(make_pair(Qt::Key_Return, fns[Qt::Key_Enter]));

	return fns;
}

bool CursorDependantKeyHandler::onKeyPressed( QShell& shell, QKeyEvent* e )
{
	static CommandMap fns = CreateCursorDependantFunctions();
	auto it = fns.find(e->key());
	if (it != end(fns))
	{
		MoveCursorAtEndIfBadPosition(shell);
		return it->second(shell, e);
	}
	return false;
}

