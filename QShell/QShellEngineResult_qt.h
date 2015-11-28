#pragma once
#include "qscriptvalue.h"
#include "QShellEngineResult.h"

class QString;

class QShellEngineResult_Qt : public QShellEngineResult
{
public:
	QShellEngineResult_Qt(QScriptValue value);
	QString ToQString() const ;
	bool IsPrintable() const ; 
	bool IsError() const ; 

	QScriptValue m_value;
};