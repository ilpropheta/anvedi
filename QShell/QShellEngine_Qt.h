#pragma once

#include <QScriptEngine>
#include "QShellEngine.h"
#include "qshell_global.h"

class QStringList;
class QString;
class QShell;
class ClearConsole;

class QSHELL_EXPORT QShellEngine_Qt : public QShellEngine
{
public:
	QShellEngine_Qt();

	std::unique_ptr<QShellEngineResult> Evaluate(const QString& toEval);
	std::unique_ptr<QShellEngineResult> MakeEmptyResult() override;
	QStringList Suggest(const QString& value);
	std::unique_ptr<QShellEngineResult> LoadScript(const QString& toEval);
	void RegisterPrintFunction(QPlainTextEdit& console);
	ClearConsole& RegisterAndGetClc();
protected:
	// just a sort of decoration of this engine
	QScriptEngine m_engine;
};