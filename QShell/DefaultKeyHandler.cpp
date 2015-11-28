#include "DefaultKeyHandler.h"
#include "QShellUtils.h"
#include "qtextcursor.h"
#include "qshell.h"

bool DefaultKeyHandler::onKeyPressed( QShell& shell, QKeyEvent* e )
{
	MoveCursorAtEndIfBadPosition(shell);
	shell.processKeyEvent(e);
	emit UnknownKeyProcessed(shell, e);
	return true;
}
