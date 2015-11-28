#pragma once

#include <memory>
#include "QShellEngineResult.h"

class QString;
class QStringList;
class QPlainTextEdit;

class QShellEngine
{
public:
	virtual void RegisterPrintFunction(QPlainTextEdit& console) = 0;
	virtual std::unique_ptr<QShellEngineResult> Evaluate(const QString& toEval) = 0;
	virtual QStringList Suggest(const QString& value) = 0;
	virtual std::unique_ptr<QShellEngineResult> LoadScript(const QString& toEval) = 0;
};
