#pragma once

#include <QObject>
#include <QColor>

class PlotInfo : public QObject
{
	Q_OBJECT
public:
	const QColor& getBackgroundColor() const;
	void setBackgroundColor(const QColor& color);
signals:
	void BackgroundColorChanged(const QColor&);
private:
	QColor backgroundColor;
};
