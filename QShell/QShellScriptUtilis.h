#pragma once

class QPlainTextEdit;
class QScriptEngine;
class QScriptValue;
class QScriptContext;

namespace QtShellScriptUtils
{
	void RegisterPrintFunctionTo(QPlainTextEdit& edit, QScriptEngine& engine);

	QScriptValue ConsolePrintFunction(QScriptContext *context, QScriptEngine *engine);
	QScriptValue ExitFunction(QScriptContext *context, QScriptEngine *engine);
	QScriptValue LoadScript(QScriptContext *context, QScriptEngine *engine);
}
