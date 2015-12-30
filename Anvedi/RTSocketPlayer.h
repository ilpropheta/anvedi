#pragma once

#include <QObject>
#include <QTimer>
#include "RTSender.h"
#include <QtNetwork>
#include <memory>
#include <vector>

class SignalData;

class RTSocketPlayer : public QObject
{
	Q_OBJECT
public:
	RTSocketPlayer(SignalData& data);
private slots:
	void Start();
public slots :
	void Stop();
private:
	SignalData& m_data;
	QLocalServer server;
	QLocalSocket* clientConnection;
	RTSender sender;
	std::vector<QString> names;
};

