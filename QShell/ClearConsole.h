#pragma once

#include <QObject>

/*	this hack merits consideration:
	you know when you evaluate an object in Js, its description gets printed
	In QtScript this means: invoking the toString public slot (if any), or a default description.
	Suppose I want a "clc" Matlab-like command, which clears the console:
	> clc
	We can declare a global "clc" object such that its toString emits a signal...
*/

class ClearConsole : public QObject
{
	Q_OBJECT
signals:
	void ClcCalled();
public slots:
	QString toString();
};

