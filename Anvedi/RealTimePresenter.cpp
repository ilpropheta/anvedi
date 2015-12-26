#include "RealTimePresenter.h"


RealTimePresenter::RealTimePresenter(SignalData& data, RTMenuInfo rtMenuActions)
	: player(data), rtMenuInfo(rtMenuActions)
{
	ui.setupUi(&rtDialog);
	
	// RT menu
	QObject::connect(rtMenuInfo.actionStartRT, SIGNAL(triggered()), this, SLOT(OnStartOrPause()));
	QObject::connect(rtMenuInfo.actionStopRT, SIGNAL(triggered()), this, SLOT(OnStop()));
	QObject::connect(rtMenuInfo.actionConfig, SIGNAL(triggered()), this, SLOT(OnConfigure()));
	
	rtMenuInfo.actionStopRT->setEnabled(false);

	player.setFileToReplay(R"(..\anvedi\json\physx.json)");
	player.setPacketCount(20);
	player.setTimerInterval(200);
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

void RealTimePresenter::OnStartOrPause()
{
	if (isRunning)
	{
		SetRunning(false);
		player.Pause();
	}
	else
	{
		SetRunning(true);
		player.Start();
	}
}

void RealTimePresenter::OnStop()
{
	SetRunning(false);
	player.Stop();
}

void RealTimePresenter::SetupRTDialog()
{
	ui.editFile->setText(player.getFileToReplay());
	ui.spinPacket->setValue(player.getPacketCount());
	ui.spinInterval->setValue(player.getTimerInterval());
}

void RealTimePresenter::SetRunning(bool running)
{
	isRunning = running;
	rtMenuInfo.actionImport->setEnabled(!running);
	rtMenuInfo.actionExport->setEnabled(!running);
	rtMenuInfo.actionClear->setEnabled(!running);
	rtMenuInfo.actionConfig->setEnabled(!running);
	rtMenuInfo.actionStartRT->setText(running ? "Pause" : "Start");
	rtMenuInfo.actionStopRT->setEnabled(running);
}
