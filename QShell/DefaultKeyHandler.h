#pragma once
#include "KeyPressHandler.h"

class DefaultKeyHandler : public IKeyPressHandler
{
	Q_OBJECT
public:
	virtual bool onKeyPressed( QShell& shell, QKeyEvent* e );
signals:
	void UnknownKeyProcessed(QShell&, QKeyEvent*);
};

