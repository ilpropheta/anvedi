#include "Utils.h"
#include <complex>
#include <QPen>

bool close(const QColor& c1, const QColor& c2)
{
	return std::sqrt(
		std::norm(c1.red() - c2.red()) +
		std::norm(c1.green() - c2.green()) +
		std::norm(c1.blue() - c2.blue())) < 50;
}

QVector<qreal> ToVector(const QVariant& var)
{
	const auto varList = var.toList();
	return ToVector(varList);
}

QVector<qreal> ToVector(const QVariantList& varList)
{
	QVector<qreal> vals; vals.reserve(varList.size());
	std::transform(varList.begin(), varList.end(), std::back_inserter(vals), [](const QVariant& v) {
		return v.toReal();
	});
	return vals;
}

QVariant ToVariant(const QVector<qreal>& vec)
{
	QVariantList vals; vals.reserve(vec.size());
	std::copy(vec.begin(), vec.end(), std::back_inserter(vals));
	return vals;
}

bool InvertPenColorIfNearTo(QPen& pen, const QColor& color)
{
	auto areClose = close(color, pen.color());
	if (areClose)
	{
		pen.setColor(invert(color));
	}
	return areClose;
}
