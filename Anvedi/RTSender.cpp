#include "RTSender.h"
#include "SignalData.h"

using namespace std;

RTSender::RTSender(SignalData& receiver)
	: receiver(receiver)
{

}

void RTSender::SetDataToSend(std::vector < std::pair<QString, QVector<qreal>>> toSend)
{
	dataToSend = move(toSend);
	currentSampleIdx = 0u;
}

void RTSender::SendData()
{
	SendData(packetCount);
}

void RTSender::SendData(int count)
{
	map<QString, QVector<qreal>> sliceToSend;
	for (const auto& elem : dataToSend)
	{
		QVector<qreal> data; data.reserve(count);
		for (auto i = 0; i < count && (currentSampleIdx + i < elem.second.size()); ++i)
		{
			data.push_back(elem.second.at(currentSampleIdx + i));
		}
		sliceToSend.emplace(elem.first, std::move(data));
	}
	currentSampleIdx += count;
	if (!sliceToSend.begin()->second.empty())
		receiver.addValues(sliceToSend);
	
	emit DataSent();
}

int RTSender::getPacketCount() const
{
	return packetCount;
}

void RTSender::setPacketCount(int pc)
{
	packetCount = pc;
}
