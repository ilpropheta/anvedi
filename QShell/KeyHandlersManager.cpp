#include "KeyHandlersManager.h"
#include "CompleterKeyHandler.h"
#include "MiscKeyHandler.h"
#include "ScriptEvaluatorKeyHandler.h"
#include "HistoryKeyHandler.h"
#include "DefaultKeyHandler.h"
#include "qshell.h"
#include "qobject.h"
#include "QShellEngineResult.h"

using namespace std;

KeyHandlersManager::KeyHandlersManager(QShell& shell)
{
	unique_ptr<CompleterKeyHandler> completerHandler (new CompleterKeyHandler(shell));
	unique_ptr<DefaultKeyHandler> defaultHandler (new DefaultKeyHandler());
	unique_ptr<ScriptEvaluatorKeyHandler> evaluatorHandler (new ScriptEvaluatorKeyHandler(shell));
	unique_ptr<HistoryKeyHandler> historyHandler (new HistoryKeyHandler());
	
	// engine-interested handlers -> can't do this (probably because of shared_ptr
	//QObject::connect(&shell, SIGNAL(NewEngine(shared_ptr<QShellEngine>)), completerHandler.get(), SLOT(onNewEngine(shared_ptr<QShellEngine>)));
	//QObject::connect(&shell, SIGNAL(NewEngine(shared_ptr<QShellEngine>)), evaluatorHandler.get(), SLOT(onNewEngine(shared_ptr<QShellEngine>)));

	// other handlers
	QObject::connect(defaultHandler.get(), SIGNAL(UnknownKeyProcessed(QShell&, QKeyEvent*)), completerHandler.get(), SLOT(onUnknownKeyProcessed(QShell&, QKeyEvent*)));
	QObject::connect(evaluatorHandler.get(), SIGNAL(TextEvaluated(const QString&, QShellEngineResult&)), historyHandler.get(), SLOT(onTextEvaluated(const QString&, QShellEngineResult&)));
	QObject::connect(&shell, SIGNAL(LoadScriptRequested(const QString&)), evaluatorHandler.get(), SLOT(onLoadScript(const QString&)));

	handlers[HandlerId::CompleterHandler] = move(completerHandler);
	handlers[HandlerId::HistoryRecorderHandler] = move(historyHandler);
	handlers[HandlerId::MiscHandler].reset(new MiscKeyHandler());
	handlers[HandlerId::CursorDependantHandler].reset(new CursorDependantKeyHandler());
	handlers[HandlerId::EvaluatorHandler] = move(evaluatorHandler);
	handlers[HandlerId::DefaultHandler] = move(defaultHandler);
}

std::vector<IKeyPressHandler*> KeyHandlersManager::GetHandlers() const
{
	vector<IKeyPressHandler*> res;
	res.reserve(handlers.size());
	transform(begin(handlers), end(handlers), back_inserter(res), 
		[](const pair<const HandlerId, unique_ptr<IKeyPressHandler>>& handler){
		return handler.second.get();
	});

	return res;
}

void KeyHandlersManager::SetEngine( std::shared_ptr<QShellEngine> engine )
{
	// I'd like to do that with a SIGNAL
	auto evalHandler = dynamic_cast<ScriptEvaluatorKeyHandler*>(handlers[HandlerId::EvaluatorHandler].get());
	if (evalHandler)
		evalHandler->onNewEngine(engine);

	auto complHandler = dynamic_cast<CompleterKeyHandler*>(handlers[HandlerId::CompleterHandler].get());
	if (complHandler)
		complHandler->onNewEngine(engine);
}