#pragma once

#include <QObject>
#include <QColor>
#include <utility>

class QCustomPlot;

class PlotInfo : public QObject
{
	Q_OBJECT
public:
	void setPlot(QCustomPlot* p);
	const QColor& getBackgroundColor() const;
	void setBackgroundColor(const QColor& color);
	const std::pair<qreal, qreal> getXRange() const;
	void setXRange(const std::pair<qreal, qreal>& range);
signals:
	void BackgroundColorChanged(const QColor&);
private:
	QColor backgroundColor;
	QCustomPlot* plot;
};
