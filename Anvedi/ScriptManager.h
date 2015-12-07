#pragma once
#include <QObject>

class SignalData;
class QShell;
class PlotInfo;

class ScriptManager : public QObject
{
	Q_OBJECT
public:
	ScriptManager(SignalData& data, PlotInfo& plot, QShell& shell);
	void InitWorkspace(SignalData& data, PlotInfo& plot, QShell& shell);
};

