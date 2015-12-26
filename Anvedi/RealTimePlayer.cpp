#include "RealTimePlayer.h"
#include "SignalData.h"
#include "PlotInfo.h"
#include "WorkspaceSerializer.h"

using namespace std;

RealTimePlayer::RealTimePlayer(SignalData& data)
	: m_data(data)
{

}

void RealTimePlayer::Start()
{
	if (isPaused)
	{
		dataTimer.start(timerInterval);
		return;
	}

	SignalData data; PlotInfo info;
	WorkspaceSerializer::Read(fileToReplay, data, info);
	timeStep = 0u;
	
	auto domain = data.getDomain()->y;
	DataMap toSet;
	vector < pair<QString, QVector<qreal>>> toSend;
	data.onSignals([&](const Signal& signal){
		toSet.emplace(signal.name, Signal{ signal.name, signal.color, signal.visible });
		toSend.emplace_back(make_pair(signal.name, std::move(signal.y)));
	});
	m_data.clear();
	m_data.add(std::move(toSet));
	m_data.setAsDomain(data.getDomain()->name);

	connect(&dataTimer, &QTimer::timeout, [toSend, this]{
		map<QString, QVector<qreal>> sliceToSend;
		for (const auto& elem : toSend)
		{
			QVector<qreal> data; data.reserve(packetCount);
			for (auto i = 0; i < packetCount && (timeStep + i < elem.second.size()); ++i)
			{
				data.push_back(elem.second.at(timeStep + i));
			}
			sliceToSend.emplace(elem.first, data);
		}
		timeStep += packetCount;
		if (!sliceToSend.begin()->second.empty())
			m_data.addValues(sliceToSend);
	});
	dataTimer.start(timerInterval);
}

void RealTimePlayer::Pause()
{
	dataTimer.stop();
	isPaused = true;
}

void RealTimePlayer::Stop()
{
	dataTimer.stop();
	this->disconnect(&dataTimer);
	isPaused = false;
	timeStep = 0.0;
}

void RealTimePlayer::setFileToReplay(QString f)
{
	fileToReplay = std::move(f);
}

const QString& RealTimePlayer::getFileToReplay() const
{
	return fileToReplay;
}

void RealTimePlayer::setTimerInterval(int ti)
{
	timerInterval = ti;
}

int RealTimePlayer::getTimerInterval() const
{
	return timerInterval;
}

void RealTimePlayer::setPacketCount(int pc)
{
	packetCount = pc;
}

int RealTimePlayer::getPacketCount() const
{
	return packetCount;
}
