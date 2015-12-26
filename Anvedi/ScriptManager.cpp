#include "ScriptManager.h"
#include "QShellEngine_Qt.h"
#include "SignalData.h"
#include "SignalHandle.h"
#include "qshell.h"
#include "PlotInfo.h"
#include "PlotHandle.h"
#include "RealTimePlayer.h"

using namespace std;

QScriptValue GraphWrapperCtor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->isCalledAsConstructor())
	{
		QScriptValue calleeData = context->callee().data();
		auto data = qobject_cast<SignalData*>(calleeData.toQObject());
		auto signalName = context->argument(0).toString();
		data->addEmptyIfNotExists(signalName);
		return engine->newQObject(new SignalHandle(move(signalName), *data), QScriptEngine::ScriptOwnership);
	}
	return context->throwError(QScriptContext::TypeError, "graph should be constructed");
}

QScriptValue SetDomain(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue calleeData = context->callee().data();
	auto data = qobject_cast<SignalData*>(calleeData.toQObject());
	if (context->argument(0).isString())
	{
		try
		{
			data->setAsDomain(context->argument(0).toString());
		}
		catch (...)
		{
			return context->throwError(QScriptContext::UnknownError, "the specified signal does not exist");
		}
		return QScriptValue::UndefinedValue;
	}
	else if (auto val = qscriptvalue_cast<SignalHandle*>(context->argument(0)))
	{
		val->SetThisAsDomain();
		return QScriptValue::UndefinedValue;
	}
	return context->throwError(QScriptContext::TypeError, "parameter is incorrect (should be either a string or a SignalHandle)");
}

QScriptValue GetDomain(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue calleeData = context->callee().data();
	auto data = qobject_cast<SignalData*>(calleeData.toQObject());
	if (auto dom = data->getDomain())
	{
		return engine->newQObject(new SignalHandle(dom->name, *data), QScriptEngine::ScriptOwnership);
	}
	else
	{
		return context->throwError(QScriptContext::TypeError, "Domain is not set yet");
	}
}

struct AnvediScriptEngine : QShellEngine_Qt
{
	AnvediScriptEngine(SignalData& data, PlotInfo& plot)
	{	
		// global properties		
		m_engine.globalObject().setProperty("plot", m_engine.newQObject(new PlotHandle(plot), QScriptEngine::ScriptOwnership));
		m_engine.globalObject().setProperty("rtPlayer", m_engine.newQObject(new RealTimePlayer(data), QScriptEngine::ScriptOwnership));
		// global functions
		m_engine.globalObject().setProperty("graph", MakeSignalDataFunction(data, GraphWrapperCtor));
		m_engine.globalObject().setProperty("SetDomain", MakeSignalDataFunction(data, SetDomain));
		m_engine.globalObject().setProperty("GetDomain", MakeSignalDataFunction(data, GetDomain));
	}

	QScriptValue MakeSignalDataFunction(SignalData& data, QScriptValue(*fun)(QScriptContext*, QScriptEngine*))
	{
		auto fnVal = m_engine.newFunction(fun);
		fnVal.setData(m_engine.newQObject(&data));
		return fnVal;
	}
};

void ScriptManager::InitWorkspace(SignalData& data, PlotInfo& plot, QShell& shell)
{
	auto scriptEngine = std::make_shared<AnvediScriptEngine>(data, plot);
	shell.SetEngine((shared_ptr<QShellEngine_Qt>)scriptEngine);
}

