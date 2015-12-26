#include "RTInteractiveSender.h"
#include "SignalData.h"
#include "RTUtils.h"

using namespace std;

RTInteractiveSender::RTInteractiveSender(SignalData& data)
	: data(data), sender(data)
{

}

RTSender* RTInteractiveSender::getSender()
{
	return &sender;
}

void RTInteractiveSender::ReStart()
{
	auto toSend = PrepareReplay(fileToReplay, data);
	sender.SetDataToSend(move(toSend));
}

const QString& RTInteractiveSender::getFileToReplay() const
{
	return fileToReplay;
}

void RTInteractiveSender::setFileToReplay(const QString& file)
{
	fileToReplay = file;
}
