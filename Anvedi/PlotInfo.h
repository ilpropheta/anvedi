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
	int getRealTimePageSize() const;
	void setRealTimePageSize(int ps);
	// axis ranges
	const std::pair<qreal, qreal> getXRange() const;
	void setXRange(const std::pair<qreal, qreal>& range);
	void autoScaleX();
	// export
	void exportToPdf(const QString& fileName, qreal w = 0, qreal h = 0);
signals:
	void BackgroundColorChanged(const QColor&);
private:
	QColor backgroundColor;
	int realTimePageSize = 1000;
	QCustomPlot* plot;
};
