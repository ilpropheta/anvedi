#pragma once

class QShell;
class QString;
class QShellEngineResult;

template<typename EventType>
void AcceptEvent(QShell& shell, EventType* e)
{
	e->accept();
	shell.ensureCursorVisible();
}

template<typename EventType>
void IgnoreEvent(QShell& shell, EventType* e)
{
	e->ignore();
}

QString StringToEvaluate(QShell& console);
int CursorBlockDifference( QShell& console );
void MoveCursorAtEndIfBadPosition( QShell& console );
bool CursorCanWrite( QShell& console );
void MoveCursorAtEnd( QShell& console );
void DeleteLastBlockContentKeepingPromptString(QShell& console);
bool IsMultiLine( const QShell& console );
void InsertEvaluationResult( QShell& console, const QShellEngineResult& result );
QString TextUnderCursor(QShell& console);
