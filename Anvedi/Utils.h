#pragma once
#include <QColor>
#include <QVector>
#include <QVariant>

class QPen;

bool close(const QColor& c1, const QColor& c2);

inline QColor invert(const QColor& col)
{
	return{ 255 - col.red(), 255 - col.green(), 255 - col.blue() };
}

bool InvertPenColorIfNearTo(QPen& pen, const QColor& color);

template<typename T>
T SaturateLeft(const T& what, const T& within)
{
	return (what < within) ? within : what;
}

template<typename T>
T SaturateRight(const T& what, const T& within)
{
	return (what > within) ? within : what;
}

QVector<qreal> ToVector(const QVariant& var);
QVector<qreal> ToVector(const QVariantList& varList);
QVariant ToVariant(const QVector<qreal>& vec);