#pragma once

#include <QObject>
#include <QTimer>

class SignalData;

class RealTimePlayer : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString file READ getFileToReplay WRITE setFileToReplay)
	Q_PROPERTY(int timerInterval READ getTimerInterval WRITE setTimerInterval)
	Q_PROPERTY(int packetCount READ getPacketCount WRITE setPacketCount)
public:
	RealTimePlayer(SignalData& data);
	void Start();
	void Pause();
	void Stop();
	// props
	void setFileToReplay(QString f);
	const QString& getFileToReplay() const;
	void setTimerInterval(int ti);
	int getTimerInterval() const;
	void setPacketCount(int pc);
	int getPacketCount() const;
private:
	SignalData& m_data;
	bool isPaused = false;
	QTimer dataTimer;
	size_t timeStep = 0u;
	// props
	QString fileToReplay;
	int timerInterval = 100;
	int packetCount = 20;
};

