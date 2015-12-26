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
	void RTDataSent();
	void RTPaused();
	void RTResumed();
	void RTStopped();
private:
	SignalData& m_data;
	bool isPaused = false;
	QTimer dataTimer;
	size_t currentSampleIdx = 0u;
	// props
	QString fileToReplay = R"(..\anvedi\json\physx.json)";
	int timerInterval = 200;
	int packetCount = 20;
};

