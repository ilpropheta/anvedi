#include "RTInteractiveSender.h"
#include "SignalData.h"
#include "Utils.h"

RTInteractiveSender::RTInteractiveSender(SignalData& data)
	: data(data), sender(data)
{
	
}

void RTInteractiveSender::Start(const QString& domainName, QVariantList signalNames)
{
	currentPacket.clear();
	DataMap toSet;
	for (const auto& name : signalNames)
	{
		Signal sign{name.toString()};
		toSet.emplace(name.toString(), sign);
		currentPacket.emplace_back(sign.name, QVector<qreal>{});
	}
	if (!toSet.count(domainName))
	{
		toSet.emplace(domainName, Signal{ domainName });
		currentPacket.emplace_back(domainName, QVector<qreal>{});
	}
	for (auto& signal : toSet)
	{
		data.addIfNotExists(std::move(signal.second));
	}
	data.setAsDomain(domainName);
}

RTSender* RTInteractiveSender::getSender()
{
	return &sender;
}

QVariantMap RTInteractiveSender::getData() const
{
	QVariantMap currData;
	for (const auto& signal : currentPacket)
	{
		currData[signal.first] = ToVariant(signal.second);
	}
	return currData;
}

void RTInteractiveSender::setData(const QVariantMap& data)
{
	currentPacket.clear();
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		currentPacket.emplace_back(it.key(), ToVector(it->toList()));
	}
	sender.SetDataToSend(currentPacket);
}


