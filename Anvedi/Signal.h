#pragma once

#include <QString>
#include <QColor>
#include <QVector>
#include "QMetaType"

struct Signal
{
	static Signal Create(QString);

	QString name;
	QColor color;  // to move
	bool visible;  // to move
	QVector<qreal> x;  // to move
	QVector<qreal> y;
};

Q_DECLARE_METATYPE(Signal)