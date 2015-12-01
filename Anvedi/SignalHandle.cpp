#include "SignalHandle.h"
#include "SignalData.h"

SignalHandle::SignalHandle(QString name, SignalData& data)
	: signalName(std::move(name)), data(data)
{

}

bool SignalHandle::isVisible() const
{
	return data.getOrInsert(signalName).visible;
}

void SignalHandle::setVisible(bool visible)
{
	data.setVisible(signalName, visible);
}

QString SignalHandle::getColor() const
{
	return data.getOrInsert(signalName).color.name();
}

void SignalHandle::setColor(const QString& color)
{
	data.setColor(signalName, color);
}

QVariant SignalHandle::getValues() const
{
	QVariantList vals;
	const auto& signal = data.get(signalName);
	for (const auto& val : signal.y)
	{
		vals.push_back(val);
	}
	return vals;
}

void SignalHandle::setValues(const QVariant& values)
{
	QVector<qreal> vals;
	QVector<qreal> x(1);
	for (const auto& item : values.toList())
	{
		vals.push_back(item.toReal());
		x.push_back(x.back() + 1);
	}
	x.pop_back();

	data.set(signalName, [&](Signal& s){
		s.y = std::move(vals);
		s.x = std::move(x);
	});
}
