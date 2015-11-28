#pragma once
#include "KeyPressHandler.h"

class MiscKeyHandler : public IKeyPressHandler
{
public:
	virtual bool onKeyPressed( QShell& shell, QKeyEvent* e );
};

class CursorDependantKeyHandler : public IKeyPressHandler
{
public:
	virtual bool onKeyPressed( QShell& shell, QKeyEvent* e );
};