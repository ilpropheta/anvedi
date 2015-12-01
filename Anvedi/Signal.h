#pragma once

#include <QString>
#include <QColor>
#include <QVector>

struct Signal
{
	static Signal Create(QString);

	QString name;
	QColor color;
	bool visible;
	QVector<qreal> x;
	QVector<qreal> y;
};
