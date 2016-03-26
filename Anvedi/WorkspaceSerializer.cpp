#include "WorkspaceSerializer.h"
#include "SignalData.h"
#include "PlotInfo.h"
#include <QFile>
#include <QJsonDocument>
#include <qjsonobject>
#include <QJsonArray>
#include <algorithm>
#include <iterator>
#include <QMessageBox>

QVector<qreal> ToVector(const QJsonArray& arr)
{
	QVector<qreal> vec; vec.reserve(arr.size()); 
	std::transform(arr.begin(), arr.end(), std::back_inserter(vec), [](const QJsonValue& value) {
		return value.toDouble(); 
	}); 
	return vec;
}

QVector<QString> ToStrVector(const QJsonArray& arr)
{
	QVector<QString> vec; vec.reserve(arr.size());
	std::transform(arr.begin(), arr.end(), std::back_inserter(vec), [](const QJsonValue& value) {
		return value.toString();
	});
	return vec;
}

Signal ReadGraph(const QJsonObject& obj)
{
	Signal signal{};

	auto it = obj.find("name");
	if (it != obj.end())
	{
		const auto sigName = it->toString();
		signal.name = std::move(sigName);

		it = obj.find("color");
		if (it != obj.end())
			signal.graphic.color = it->toString();
		
		it = obj.find("visible");
		if (it != obj.end())
			signal.graphic.visible = it->toBool();
		
		it = obj.find("range");
		if (it != obj.end())
		{
			auto vec = ToVector(it->toArray());
			signal.graphic.rangeLower = vec.front();
			signal.graphic.rangeUpper = vec.back();
		}
		
		it = obj.find("values");
		if (it != obj.end() && it->isArray())
		{
			signal.y = ToVector(it->toArray());
		}
		
		it = obj.find("ticks");
		if (it != obj.end() && it->isArray())
		{
			signal.graphic.ticks = ToVector(it->toArray());
		}

		it = obj.find("tickLabels");
		if (it != obj.end() && it->isArray())
		{
			signal.graphic.tickLabels = ToStrVector(it->toArray());
		}
	}

	return signal;
}

void Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo, std::function<void(Signal&&)> onSignal, std::function<void()> onAfterSignalRead)
{
	QFile in(fileName);
	if (!in.open(QIODevice::ReadOnly | QFile::Text))
	{
		auto mex = QString("Cannot open specified file -> %1").arg(fileName);
		QMessageBox::critical(nullptr, "Anvedi", mex);
		throw std::exception(mex.toUtf8().data());
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
		for (const auto& elem : graphIt->toArray())
		{
			if (elem.isObject())
			{
				onSignal(ReadGraph(elem.toObject()));
			}
		}
	}
	
	onAfterSignalRead();

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

void WorkspaceSerializer::Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo)
{
	DataMap newData;
	::Read(fileName, data, plotInfo, [&](Signal&& signal){
		auto signalName = signal.name;
		newData.emplace(std::move(signalName), std::move(signal));
	}, [&]{
		data.add(std::move(newData));
	});
}

void WorkspaceSerializer::Read(const QString& fileName, SignalData& data, PlotInfo& plotInfo, std::function<void(Signal&&)> onSignal)
{
	::Read(fileName, data, plotInfo, onSignal, []{});
}

QJsonArray ToJsonArray(const QVector<qreal>& vec)
{
	QJsonArray arr; 
	std::copy(vec.begin(), vec.end(), std::back_inserter(arr));
	return arr;
}

QJsonArray ToJsonArray(const QVector<QString>& vec)
{
	QJsonArray arr;
	std::copy(vec.begin(), vec.end(), std::back_inserter(arr));
	return arr;
}

void WorkspaceSerializer::Write(const QString& fileName, const SignalData& data, const PlotInfo& plotInfo, bool writeValues /*= true*/)
{
	QJsonObject json;
	json["background"] = plotInfo.getBackgroundColor().name();
	if (writeValues)
	{
		if (auto domain = data.getDomain())
			json["domain"] = domain->name;
	}
	
	QJsonArray signArray;
	data.onSignals([&](const Signal& signal){
		QJsonObject signObj;
		signObj["name"] = signal.name;
		signObj["color"] = signal.graphic.color.name();
		signObj["visible"] = signal.graphic.visible;
		signObj["range"] = ToJsonArray(QVector < qreal > {signal.graphic.rangeLower, signal.graphic.rangeUpper});
		if (writeValues)
			signObj["values"] = ToJsonArray(signal.y);
		signObj["ticks"] = ToJsonArray(signal.graphic.ticks);
		signObj["tickLabels"] = ToJsonArray(signal.graphic.tickLabels);
		signArray.push_back(std::move(signObj));
	});
	json["signals"] = signArray;
	
	QJsonDocument saveDoc(json);
	QFile outF(fileName);
	outF.open(QIODevice::WriteOnly);
	outF.write(saveDoc.toJson(QJsonDocument::Indented));
}