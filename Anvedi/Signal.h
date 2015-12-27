#pragma once

#include <QString>
#include <QColor>
#include <QVector>
#include "QMetaType"

struct SignalGraphic
{
	QColor color;
	bool visible;
	double rangeLower, rangeUpper;
	QVector<qreal> ticks;
	QVector<QString> tickLabels;

	bool operator==(const SignalGraphic& other) const;
};

struct Signal
{
	QString name;
	QVector<qreal> y;
	SignalGraphic graphic; // (*)

	bool operator==(const Signal& other) const;
};

/* (*)   
   Important Note: this is a design shortcut because domain data (e.g. y values)
				   should be completely detached from the graphic info.
*/


Q_DECLARE_METATYPE(Signal)