#pragma once

class SignalData;
class QShell;
class PlotInfo;

class ScriptManager
{
public:
	static void InitWorkspace(SignalData& data, PlotInfo& plot, QShell& shell);
};

