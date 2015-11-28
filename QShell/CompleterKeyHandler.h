#pragma once
#include "KeyPressHandler.h"
#include "qobjectdefs.h"
#include "QShellEngine.h"

class QAbstractItemModel;
class QShellCompleter;

class CompleterKeyHandler : public IKeyPressHandler
{
	Q_OBJECT
public:
	CompleterKeyHandler(QShell& shell);

	virtual bool onKeyPressed( QShell& shell, QKeyEvent* e );
public slots:
	void onUnknownKeyProcessed(QShell& shell, QKeyEvent* e);
	void onNewEngine(std::shared_ptr<QShellEngine>);
	void onInsertSuggestion(const QString& suggestion);
private:
	void ShowCompletion(QShell& shell);
	QAbstractItemModel* intelliNaiveModel(const QString& rootObj);
	
	QShellCompleter*			  completer;
	QShell&						  shell;	
	std::shared_ptr<QShellEngine> engine;
};

