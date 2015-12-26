#pragma once
#include <QObject>
#include "RealTimePlayer.h"
#include "ui_rtConfig.h"
#include <QDialog>

struct RTMenuInfo
{
	QAction* actionStartRT;
	QAction* actionStopRT;
	QAction* actionConfig;
	// data menu
	QAction* actionImport;
	QAction* actionExport;
	QAction* actionClear;
};

class RealTimePresenter : public QObject
{
	Q_OBJECT
public:
	RealTimePresenter(SignalData& data, RTMenuInfo rtMenuActions);
public slots:
	void OnConfigure();
	void OnStart();
	void OnPause();
	void OnStop();
private:
	void SetupRTDialog();
	void SetRunning(bool enable);

	Ui::Dialog ui;
	QDialog rtDialog;
	RTMenuInfo rtMenuInfo;
	RealTimePlayer player;
};

