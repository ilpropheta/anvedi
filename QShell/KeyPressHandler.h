#pragma once
#include "qobjectdefs.h"
#include "qobject.h"

class QKeyEvent;
class QShell;

class IKeyPressHandler : public QObject
{
	Q_OBJECT
public:
	~IKeyPressHandler() {}
	virtual bool onKeyPressed(QShell& shell, QKeyEvent* e) = 0;
};