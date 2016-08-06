#pragma once
#include <QObject>
#include "RTSender.h"

class SignalData;

class RTInteractiveFileSender : public QObject
{
	Q_OBJECT
	Q_PROPERTY(RTSender* sender READ getSender)
	Q_PROPERTY(QString fileToReplay READ getFileToReplay WRITE setFileToReplay)
public:
	RTInteractiveFileSender(SignalData& data);

	RTSender* getSender();
	const QString& getFileToReplay() const;
	void setFileToReplay(const QString&);
public slots:
	void ReStart();
private:
	SignalData& data;
	RTSender sender;
	QString fileToReplay = R"(C:\Users\Marco\Source\Repos\anvedi\Anvedi\json\physx-0204.json)";
};

