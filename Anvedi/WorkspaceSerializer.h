#pragma once
#include <QString>

class SignalData;
class PlotInfo;

class WorkspaceSerializer
{
public:
	static void Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo);
	static void Write(const QString& fileName, const SignalData& data, const PlotInfo& plotInfo);
};

