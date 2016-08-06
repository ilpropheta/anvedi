#pragma once

#include <QObject>
#include <QTimer>
#include "RTSender.h"
#include <memory>

class SignalData;

class RealTimePlayer : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString file READ getFileToReplay WRITE setFileToReplay)
	Q_PROPERTY(int timerInterval READ getTimerInterval WRITE setTimerInterval)
	Q_PROPERTY(int packetCount READ getPacketCount WRITE setPacketCount)
public:
	RealTimePlayer(SignalData& data);
	
	// props
	void setFileToReplay(QString f);
	const QString& getFileToReplay() const;
	void setTimerInterval(int ti);
	int getTimerInterval() const;
	void setPacketCount(int pc);
	int getPacketCount() const;
public slots:
	void Start();
	void Pause();
	void Stop();
signals:
	void RTStarted();
	void RTPaused();
	void RTResumed();
	void RTStopped();
private:
	void PrepareSender();

	SignalData& m_data;
	bool isPaused = false;
	QTimer dataTimer;
	// props
	QString fileToReplay = R"(C:\Users\Marco\Source\Repos\anvedi\Anvedi\json\physx-0204.json)";
	int timerInterval = 200;
	RTSender sender;
};

