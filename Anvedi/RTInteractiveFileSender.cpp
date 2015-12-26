#include "RTInteractiveFileSender.h"
#include "SignalData.h"
#include "RTUtils.h"

using namespace std;

RTInteractiveFileSender::RTInteractiveFileSender(SignalData& data)
	: data(data), sender(data)
{

}

RTSender* RTInteractiveFileSender::getSender()
{
	return &sender;
}

void RTInteractiveFileSender::ReStart()
{
	auto toSend = PrepareReplay(fileToReplay, data);
	sender.SetDataToSend(move(toSend));
}

const QString& RTInteractiveFileSender::getFileToReplay() const
{
	return fileToReplay;
}

void RTInteractiveFileSender::setFileToReplay(const QString& file)
{
	fileToReplay = file;
}
