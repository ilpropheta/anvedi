#include "SignalHandle.h"
#include "SignalData.h"
#include "Utils.h"

SignalHandle::SignalHandle(QString name, SignalData& data)
	: signalName(std::move(name)), data(data)
{

}

bool SignalHandle::isVisible() const
{
	return data.get(signalName).graphic.visible;
}

void SignalHandle::setVisible(bool visible)
{
	data.setVisible(signalName, visible);
}

QString SignalHandle::getColor() const
{
	return data.get(signalName).graphic.color.name();
}

void SignalHandle::setColor(const QString& color)
{
	data.setColor(signalName, color);
}

QVariant SignalHandle::getValues() const
{
	return ToVariant(data.get(signalName).y);
}

void SignalHandle::setValues(const QVariant& values)
{
	data.setValues(signalName, ToVector(values));
}

void SignalHandle::SetThisAsDomain() 
{
	data.setAsDomain(signalName);
}

QString SignalHandle::toString() const
{
	return QString("Handle to signal '%1'").arg(signalName);
}

QVariant SignalHandle::getRange() const
{
	const auto& signal = data.get(signalName);
	return QVariantList{ signal.graphic.rangeLower, signal.graphic.rangeUpper };
}

void SignalHandle::setRange(const QVariant& values)
{
	if ((values.type() == QVariant::String) && (values.toString() == "auto"))
	{
		data.setAutoRange(signalName);
	}
	else
	{
		const auto rangeToVec = ToVector(values);
		if (rangeToVec.size() == 2)
		{
			data.setRange(signalName, rangeToVec.front(), rangeToVec.back());
		}
	}
}

QVariant SignalHandle::getTicks() const
{
	return ToVariant(data.get(signalName).graphic.ticks);
}

void SignalHandle::setTicks(const QVariant& values)
{
	data.setTicks(signalName, ToVector(values));
}
