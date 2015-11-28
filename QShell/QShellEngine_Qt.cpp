#include "QShellEngine_Qt.h"
#include "qscriptvalueiterator.h"
#include "qshell.h"
#include "QShellEngineResult_qt.h"
#include "QShellScriptUtilis.h"


QShellEngine_Qt::QShellEngine_Qt()
{
	m_engine.globalObject().setProperty("exit", m_engine.newFunction(QtShellScriptUtils::ExitFunction));
	m_engine.globalObject().setProperty("load", m_engine.newFunction(QtShellScriptUtils::LoadScript, 1));
}

void QShellEngine_Qt::RegisterPrintFunction(QPlainTextEdit& console)
{
	QtShellScriptUtils::RegisterPrintFunctionTo(console, m_engine);
}

std::unique_ptr<QShellEngineResult> QShellEngine_Qt::Evaluate( const QString& toEval )
{
	auto result = m_engine.evaluate(toEval);
	return std::unique_ptr<QShellEngineResult>(new QShellEngineResult_Qt(result));
}

QScriptValue ScriptValueSuggestion(QScriptEngine& engine, const QString& value)
{
	static const QString intelliFn = "({ Intelli: function(something) { var local=new Array(); var j=0; for (var i in something) local[j++]=i; return local; } }).Intelli";
	return engine.evaluate(QString("%1(%2)").arg(intelliFn).arg(value));
}

QStringList QShellEngine_Qt::Suggest( const QString& value )
{
	QStringList queryResult;
	auto scriptResult = ScriptValueSuggestion(m_engine, value);
	if (false == scriptResult.isError())
	{
		QScriptValueIterator scriptValueIterator(scriptResult);
		scriptValueIterator.toBack();
		scriptValueIterator.previous();
		while (scriptValueIterator.hasPrevious()) 
		{
			scriptValueIterator.previous();
			queryResult.append(scriptValueIterator.value().toString());
		}
		queryResult.sort();
	}

	return queryResult;
}

std::unique_ptr<QShellEngineResult> QShellEngine_Qt::LoadScript( const QString& toEval )
{
	return Evaluate("load('" + toEval + "')");
}


