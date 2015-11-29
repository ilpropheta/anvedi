#include "qshell.h"
#include "QShellUtils.h"
#include "QShellEngine_Qt.h"


QShell::QShell( QWidget* parent, const QString& promptString /*= "Q> "*/ )
	: QPlainTextEdit(parent), prompt(promptString), commands(*this), handlersMananger(*this)
{
	setPlainText(prompt);
	MoveCursorAtEnd(*this);
	commands.SetHandlers(handlersMananger.GetHandlers());
	QObject::connect(&commands, SIGNAL(ScriptLoadRequested(const QString&)), this, SIGNAL(LoadScriptRequested(const QString&)));
	auto engine = std::make_shared<QShellEngine_Qt>();
	engine->RegisterPrintFunction(*this);
	commands.SetClc(engine->RegisterAndGetClc());
	SetEngine(engine);
}

void QShell::SetPrompt( const QString& newPrompt )
{
	prompt = newPrompt;
}

void QShell::SetHandlers( HandlersVec handlers )
{
	commands.SetHandlers(std::move(handlers));
}

const QString& QShell::Prompt() const
{
	return prompt;
}

void QShell::SetEngine( std::shared_ptr<QShellEngine> engine )
{
	handlersMananger.SetEngine(engine);
}

void QShell::SetEngine(std::shared_ptr<QShellEngine_Qt> engine)
{
	engine->RegisterPrintFunction(*this);
	commands.SetClc(engine->RegisterAndGetClc());
	handlersMananger.SetEngine(engine);
}

// *************************************************************** handlers
void QShell::keyPressEvent( QKeyEvent *e )
{
	commands.onKeyPress(e);
}

void QShell::wheelEvent( QWheelEvent *e )
{
	commands.onMouseWheelEvent(e);
}

void QShell::contextMenuEvent( QContextMenuEvent *e )
{
	commands.onContextMenuEvent(e);
}

void QShell::insertFromMimeData( const QMimeData * source )
{
	commands.onInsertFromMimeData(source);
}
// *************************************************************** handlers

// *************************************************************** QPlainTextEdit forwards
void QShell::processKeyEvent( QKeyEvent* e )
{
	QPlainTextEdit::keyPressEvent(e);
}

void QShell::processWheelEvent( QWheelEvent* e )
{
	QPlainTextEdit::wheelEvent(e);
}

// *************************************************************** QPlainTextEdit forwards