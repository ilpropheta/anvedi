#pragma once
#include <QString>
#include <functional>
#include "Signal.h"

class SignalData;
class PlotInfo;

class WorkspaceSerializer
{
public:
	static void Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo);
	static void Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo, std::function<void(Signal&&)> onSignal);
	static void Write(const QString& fileName, const SignalData& data, const PlotInfo& plotInfo);
};

