#pragma once

#include <QString>
#include <QColor>
#include <QVector>
#include "QMetaType"

struct Signal
{
	QString name;
	QColor color;
	bool visible;
	QVector<qreal> y;
};

Q_DECLARE_METATYPE(Signal)