#include "RTUtils.h"
#include "SignalData.h"
#include "PlotInfo.h"
#include "WorkspaceSerializer.h"

using namespace std;

DataToReplay PrepareReplay(const QString& file, SignalData& signalData)
{
	SignalData data; PlotInfo info;
	WorkspaceSerializer::Read(file, data, info);

	DataMap toSet;
	vector < pair<QString, QVector<qreal>>> toSend;
	data.onSignals([&](const Signal& signal){
		toSet.emplace(signal.name, Signal{ signal.name, {}, signal.graphic });
		toSend.emplace_back(make_pair(signal.name, std::move(signal.y)));
	});

	signalData.clear();
	signalData.add(toSet);
	signalData.setAsDomain(data.getDomain()->name);

	return toSend;
}
