#pragma once
#include <QObject>

class SignalData;
class QShell;

class ScriptManager : public QObject
{
	Q_OBJECT
public:
	ScriptManager(SignalData& data, QShell& shell);
	void InitWorkspace(SignalData& data, QShell& shell);
};

