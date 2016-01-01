#pragma once
#include <QObject>
#include <vector>
#include <utility>
#include <QVector>
#include "RTUtils.h"

class SignalData;

class RTSender : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int packetCount READ getPacketCount WRITE setPacketCount)
public:
	RTSender(SignalData& receiver);

	void SetDataToSend(DataToReplay toSend);
	int getPacketCount() const;
	void setPacketCount(int pc);
public slots:
	void SendData();
	void SendData(int count);
signals:
	void DataSent();
	void NoMoreData();
private:
	DataToReplay dataToSend;
	SignalData& receiver;
	size_t currentSampleIdx = 0u;
	int packetCount = 20;
};

