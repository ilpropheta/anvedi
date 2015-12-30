#include "RealTimePresenter.h"

RealTimePresenter::RealTimePresenter(SignalData& data, RTMenuInfo rtMenuActions)
	: player(data), socketPlayer(data), rtMenuInfo(rtMenuActions)
{
	ui.setupUi(&rtDialog);
	
	// RT menu -> player
	QObject::connect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Start()));
	QObject::connect(rtMenuInfo.actionStopRT, SIGNAL(triggered()), &player, SLOT(Stop()));
	// RT config -> this
	QObject::connect(rtMenuInfo.actionConfig, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	// player -> this
	QObject::connect(&player, SIGNAL(RTStarted()), this, SLOT(OnStart()));
	QObject::connect(&player, SIGNAL(RTPaused()), this, SLOT(OnPause()));
	QObject::connect(&player, SIGNAL(RTResumed()), this, SLOT(OnStart()));
	QObject::connect(&player, SIGNAL(RTStopped()), this, SLOT(OnStop()));

	rtMenuInfo.actionStopRT->setEnabled(false);
}

void RealTimePresenter::OnConfigure()
{
	if (SetupRTDialog(), rtDialog.exec() == QDialog::Accepted)
	{
		player.setFileToReplay(ui.editFile->text());
		player.setPacketCount(ui.spinPacket->value());
		player.setTimerInterval(ui.spinInterval->value());
	}
}

void RealTimePresenter::OnStart()
{
	SetRunning(true);
	QObject::disconnect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Start()));
	QObject::connect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Pause()));
}

void RealTimePresenter::OnPause()
{
	SetRunning(false);
	QObject::disconnect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Pause()));
	QObject::connect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Start()));
	rtMenuInfo.actionStopRT->setEnabled(true);
}

void RealTimePresenter::OnStop()
{
	SetRunning(false);
	QObject::disconnect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Pause()));
	QObject::connect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), &player, SLOT(Start()));
}

void RealTimePresenter::SetupRTDialog()
{
	ui.editFile->setText(player.getFileToReplay());
	ui.spinPacket->setValue(player.getPacketCount());
	ui.spinInterval->setValue(player.getTimerInterval());
}

void RealTimePresenter::SetRunning(bool running)
{
	rtMenuInfo.actionImport->setEnabled(!running);
	rtMenuInfo.actionExport->setEnabled(!running);
	rtMenuInfo.actionClear->setEnabled(!running);
	rtMenuInfo.actionConfig->setEnabled(!running);
	rtMenuInfo.actionStartRT->setText(running ? "Pause" : "Start");
	rtMenuInfo.actionStopRT->setEnabled(running);
}
