#pragma once
#include <memory>
#include <map>
#include "KeyPressHandler.h"
#include <vector>
#include "QShellEngine.h"

class QShell;

class KeyHandlersManager
{
public:
	enum class HandlerId
	{
		CompleterHandler=0, HistoryRecorderHandler, MiscHandler, CursorDependantHandler, 
		EvaluatorHandler, DefaultHandler 
	};

	KeyHandlersManager(QShell& shell);

	std::vector<IKeyPressHandler*> GetHandlers() const;
	void SetEngine( std::shared_ptr<QShellEngine> engine );

private:
	std::map<HandlerId, std::unique_ptr<IKeyPressHandler>> handlers;
};

