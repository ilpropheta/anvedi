#include "qmlTick.h"

const QString& qmlTick::getFont() const
{
	return font;
}

void qmlTick::setFont(const QString& f)
{
	font = f;
}

QVariantList qmlTick::getVector() const
{
	QVariantList vals; vals.reserve(vector.size());
	std::copy(vector.begin(), vector.end(), std::back_inserter(vals));
	return vals;
}

void qmlTick::setVector(const QVariantList& varList)
{
	vector.clear(); vector.reserve(varList.size());
	std::transform(varList.begin(), varList.end(), std::back_inserter(vector), [](const QVariant& v) {
		return v.toReal();
	});
}

QVariantList qmlTick::getLabels() const
{
	QVariantList vals; vals.reserve(labels.size());
	std::copy(labels.begin(), labels.end(), std::back_inserter(vals));
	return vals;
}

void qmlTick::setLabels(const QVariantList& varList)
{
	labels.clear(); labels.reserve(varList.size());
	std::transform(varList.begin(), varList.end(), std::back_inserter(labels), [](const QVariant& v) {
		return v.toString();
	});
}

const QVector<qreal>& qmlTick::getTickVector() const
{
	return vector;
}

const QVector<QString>& qmlTick::getTickLabels() const
{
	return labels;
}
