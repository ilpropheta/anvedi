#pragma once
#include <QObject>
#include "ui_signalInfo.h"

class SignalData;

class SignalInfoPresenter : public QObject
{
	Q_OBJECT
public:
	SignalInfoPresenter(SignalData& data);
	void Config(const QString& signalName);
private:
	Ui::SignalInfoDlg ui;
	QDialog configDialog;
	SignalData& data;
};

