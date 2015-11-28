#include "QShellUtils.h"
#include "qshell.h"
#include <QString>
#include <QTextBlock>
#include <QScriptValue>
#include "QShellEngineResult.h"


QString StringToEvaluate( QShell& console )
{
	auto currentLine = console.document()->lastBlock().text();
	return currentLine.trimmed().remove(0, console.Prompt().size());
}

void MoveCursorAtEndIfBadPosition( QShell& console )
{
	if (!CursorCanWrite(console))
		MoveCursorAtEnd(console);
}

bool CursorCanWrite( QShell& console )
{
	auto minValidPosition = console.document()->lastBlock().position() + console.Prompt().size();
	auto start = console.textCursor().selectionStart();
	auto end = console.textCursor().selectionEnd();

	return start >= minValidPosition && end >= minValidPosition;
}

int CursorBlockDifference( QShell& console )
{
	auto currentCursorPosition = console.textCursor().position();
	auto lastBlockPosition = console.document()->lastBlock().position();
	lastBlockPosition += console.Prompt().size();
	return currentCursorPosition - lastBlockPosition;
}

void MoveCursorAtEnd( QShell& console ) 
{
	auto cursor = console.textCursor();
	cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
	console.setTextCursor(cursor);
}

void DeleteLastBlockContentKeepingPromptString( QShell& console )
{
	MoveCursorAtEnd(console);
	console.moveCursor( QTextCursor::EndOfBlock, QTextCursor::MoveAnchor );
	console.moveCursor( QTextCursor::StartOfBlock, QTextCursor::KeepAnchor );

	for (int i=0; i<console.Prompt().size(); i++)
		console.moveCursor( QTextCursor::Right, QTextCursor::KeepAnchor );

	console.textCursor().removeSelectedText();
}

bool IsMultiLine( const QShell& console )
{
	return console.document()->lastBlock().lineCount()>1;
}

void InsertEvaluationResult( QShell& console, const QShellEngineResult& result ) 
{
	// this stuff should be done by caring the syntax highlighting rules...
	QString html;
	if (result.IsPrintable())
	{
		html += QString("<p>%1</p><p></p>")
			.arg( 
				result.IsError() ? 
				QString("<font color='red'>%1</font>").arg(result.ToQString()) : 
				result.ToQString()
			);
	}
	console.appendHtml(html);
	console.insertPlainText(console.Prompt());
	console.document()->clearUndoRedoStacks();
}

QString TextUnderCursor(QShell& console)
{
	auto pos = console.textCursor().positionInBlock();
	return console.document()->lastBlock()
		.text().left(pos)
		.split(QChar(8232)).last()
		.split(" ").last();
}


