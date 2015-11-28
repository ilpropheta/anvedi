#pragma once
#include "KeyPressHandler.h"
#include "HistoryRecorder.h"
#include "qshell.h"

class QShellEngineResult;

class HistoryKeyHandler : public IKeyPressHandler
{
	Q_OBJECT
public:
	virtual bool onKeyPressed( QShell& shell, QKeyEvent* e );
public slots:
	void onTextEvaluated(const QString& text, QShellEngineResult& res);
private:
	HistoryRecorder<QString> history;
};

