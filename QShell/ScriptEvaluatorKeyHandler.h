#pragma once
#include "KeyPressHandler.h"
#include <memory>
#include "QShellEngine.h"
#include "QtCore\qobjectdefs.h"

class QShell;

class ScriptEvaluatorKeyHandler : public IKeyPressHandler
{
	Q_OBJECT
public:
	ScriptEvaluatorKeyHandler(QShell& s);
	virtual bool onKeyPressed( QShell& shell, QKeyEvent* e );
public slots:
	void onNewEngine(std::shared_ptr<QShellEngine>);
	void onLoadScript(const QString& file);
signals:
	void TextEvaluated(const QString&, QShellEngineResult&);
private:
	std::shared_ptr<QShellEngine> engine;
	QShell& shell;
};

