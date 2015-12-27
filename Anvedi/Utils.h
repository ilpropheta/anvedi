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

QVector<qreal> ToVector(const QVariant& var);
QVector<qreal> ToVector(const QVariantList& varList);
QVariant ToVariant(const QVector<qreal>& vec);