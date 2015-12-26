#include "qmlTick.h"
#include "Utils.h"

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
	return ToVariant(vector).toList();
}

void qmlTick::setVector(const QVariantList& varList)
{
	vector = ToVector(varList);
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
