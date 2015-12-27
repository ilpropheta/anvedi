#include "WorkspaceSerializer.h"
#include "SignalData.h"
#include "PlotInfo.h"
#include <QFile>
#include <QJsonDocument>
#include <qjsonobject>
#include <QJsonArray>
#include <algorithm>
#include <iterator>

QVector<qreal> ToVector(const QJsonArray& arr)
{
	QVector<qreal> vec; vec.reserve(arr.size()); 
	std::transform(arr.begin(), arr.end(), std::back_inserter(vec), [](const QJsonValue& value) {
		return value.toDouble(); 
	}); 
	return vec;
}

void ReadGraph(const QJsonObject& obj, DataMap& data)
{
	auto it = obj.find("name");
	if (it != obj.end())
	{
		const auto sigName = it->toString();
		Signal signal{sigName};

		it = obj.find("color");
		if (it != obj.end())
			signal.graphic.color = it->toString();
		it = obj.find("visible");
		if (it != obj.end())
			signal.graphic.visible = it->toBool();
		it = obj.find("values");
		if (it != obj.end() && it->isArray())
		{
			signal.y = ToVector(it->toArray());
		}
		data.emplace(std::move(sigName), std::move(signal));
	}
}

void WorkspaceSerializer::Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo)
{
	QFile in(fileName);
	if (!in.open(QIODevice::ReadOnly | QFile::Text))
	{
		return; // should report error properly
	}
	const auto json = QJsonDocument::fromJson(in.readAll()).object();

	auto backgroundIt = json.find("background");
	if (backgroundIt != json.end())
	{
		plotInfo.setBackgroundColor(backgroundIt->toString());
	}
	auto graphIt = json.find("signals");
	if ((graphIt != json.end()) && graphIt->isArray())
	{
		DataMap newData;
		for (const auto& elem : graphIt->toArray())
		{
			if (elem.isObject())
			{
				ReadGraph(elem.toObject(), newData);
			}
		}
		data.add(std::move(newData));
	}
	auto domainIt = json.find("domain");
	if (domainIt != json.end())
	{
		const auto domainName = domainIt->toString();
		try
		{
			data.setAsDomain(domainName);
		}
		catch (const std::exception&){}
	}
}

QJsonArray ToJsonArray(const QVector<qreal>& vec)
{
	QJsonArray arr; 
	std::copy(vec.begin(), vec.end(), std::back_inserter(arr));
	return arr;
}

void WorkspaceSerializer::Write(const QString& fileName, const SignalData& data, const PlotInfo& plotInfo)
{
	QJsonObject json;
	json["background"] = plotInfo.getBackgroundColor().name();
	if (auto domain = data.getDomain())
		json["domain"] = domain->name;
	
	QJsonArray signArray;
	data.onSignals([&](const Signal& signal){
		QJsonObject signObj;
		signObj["name"] = signal.name;
		signObj["color"] = signal.graphic.color.name();
		signObj["visible"] = signal.graphic.visible;
		signObj["values"] = ToJsonArray(signal.y);
		signArray.push_back(std::move(signObj));
	});
	json["signals"] = signArray;
	
	QJsonDocument saveDoc(json);
	QFile outF(fileName);
	outF.open(QIODevice::WriteOnly);
	outF.write(saveDoc.toJson(QJsonDocument::Indented));
}
