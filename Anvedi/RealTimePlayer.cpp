#include "RealTimePlayer.h"
#include "SignalData.h"
#include "PlotInfo.h"
#include "WorkspaceSerializer.h"
#include "RTUtils.h"

using namespace std;

RealTimePlayer::RealTimePlayer(SignalData& data)
	: m_data(data), sender(m_data)
{
	connect(&dataTimer, SIGNAL(timeout()), &sender, SLOT(SendData()));
}

void RealTimePlayer::PrepareSender()
{
	auto toSend = PrepareReplay(fileToReplay, m_data);
	sender.SetDataToSend(move(toSend));
}

void RealTimePlayer::Start()
{
	if (isPaused)
	{
		dataTimer.start(timerInterval);
		emit RTResumed();
		return;
	}
	
	PrepareSender();
	emit RTStarted();
	dataTimer.start(timerInterval);	
}

void RealTimePlayer::Pause()
{
	dataTimer.stop();
	isPaused = true;
	emit RTPaused();
}

void RealTimePlayer::Stop()
{
	dataTimer.stop(); 
	isPaused = false;
	emit RTStopped();
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
	sender.setPacketCount(pc);
}

int RealTimePlayer::getPacketCount() const
{
	return sender.getPacketCount();
}
