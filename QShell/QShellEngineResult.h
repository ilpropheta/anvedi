#pragma once

class QString;

class QShellEngineResult
{
public:
	virtual ~QShellEngineResult() {}
	virtual QString ToQString() const = 0;
	virtual bool IsPrintable() const = 0;
	virtual bool IsError() const = 0;
};

