#include "SignalHandle.h"
#include "SignalData.h"

SignalHandle::SignalHandle(QString name, SignalData& data)
	: signalName(std::move(name)), data(data)
{

}

bool SignalHandle::isVisible() const
{
	return data.get(signalName).visible;
}

void SignalHandle::setVisible(bool visible)
{
	data.setVisible(signalName, visible);
}

QString SignalHandle::getColor() const
{
	return data.get(signalName).color.name();
}

void SignalHandle::setColor(const QString& color)
{
	data.setColor(signalName, color);
}

QVariant SignalHandle::getValues() const
{
	const auto& signal = data.get(signalName);
	QVariantList vals; vals.reserve(signal.y.size());
	std::copy(signal.y.begin(), signal.y.end(), std::back_inserter(vals));
	return vals;
}

void SignalHandle::setValues(const QVariant& values)
{
	const auto varList = values.toList();
	QVector<qreal> vals; vals.reserve(varList.size());
	std::transform(varList.begin(), varList.end(), std::back_inserter(vals), [](const QVariant& v) {
		return v.toReal();
	});
	data.setValues(signalName, std::move(vals));
}

void SignalHandle::SetThisAsDomain() 
{
	data.setAsDomain(signalName);
}

QString SignalHandle::toString() const
{
	return QString("Handle to signal '%1'").arg(signalName);
}
