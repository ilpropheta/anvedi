#pragma once
#include "RTSender.h"
#include <QObject>
#include <QVariant>

class RTInteractiveSender : public QObject
{
	Q_OBJECT
	Q_PROPERTY(RTSender* sender READ getSender)
	Q_PROPERTY(QVariantMap data READ getData WRITE setData)
public:
	RTInteractiveSender(SignalData& data);

	RTSender* getSender();
	QVariantMap getData() const;
	void setData(const QVariantMap& data);
public slots:
	void Start(const QString& domainName, QVariantList signalNames);
private:
	SignalData& data;
	RTSender sender;
	DataToReplay currentPacket;
};
