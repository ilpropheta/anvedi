#include "ClearConsole.h"

QString ClearConsole::toString()
{
	emit ClcCalled(); // -> will be received to clear the console
	return QString();
}

