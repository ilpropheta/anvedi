#include "QShellScriptUtilis.h"
#include "qplaintextedit.h"
#include "QShellEngine.h"
#include "qscriptengine.h"
#include "qscriptcontext.h"
#include "qapplication.h"
#include "qtextstream.h"

using namespace QtShellScriptUtils;

void QtShellScriptUtils::RegisterPrintFunctionTo( QPlainTextEdit& edit, QScriptEngine& engine )
{
	QScriptValue printFunc = engine.newFunction(ConsolePrintFunction);
	printFunc.setData(engine.newQObject(&edit));
	engine.globalObject().setProperty("print", printFunc);
}

QScriptValue QtShellScriptUtils::ConsolePrintFunction( QScriptContext *context, QScriptEngine *engine )
{
	QString result;
	for (int i = 0; i < context->argumentCount(); ++i) {
		if (i > 0)
			result.append(" ");
		result.append(context->argument(i).toString());
	}

	QScriptValue calleeData = context->callee().data();
	QPlainTextEdit *edit = qobject_cast<QPlainTextEdit*>(calleeData.toQObject());
	edit->insertPlainText("\n"+result);

	return engine->undefinedValue();
}

QScriptValue QtShellScriptUtils::ExitFunction( QScriptContext *context, QScriptEngine *engine )
{
	QApplication::exit(0);
	return engine->undefinedValue();
}

QScriptValue QtShellScriptUtils::LoadScript( QScriptContext *context, QScriptEngine *engine )
{
	QString fileName = context->argument(0).toString();

	for (int i = 0; i < context->argumentCount(); ++i) {	
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly))
			return context->throwError(QString::fromLatin1("Could not open %0 for reading...").arg(fileName));

		QTextStream ts(&file);
		QString contents = ts.readAll();
		file.close();
		QScriptContext *pc = context->parentContext();
		context->setActivationObject(pc->activationObject());
		context->setThisObject(pc->thisObject());
		QScriptValue ret = engine->evaluate(contents);
		if (engine->hasUncaughtException())
			return ret;
	}
	return QString::fromLatin1("Script %0 loaded correctly!").arg(fileName);
}
