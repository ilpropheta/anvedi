#pragma once

#include <QObject>
#include <QColor>
#include <utility>

class QCustomPlot;

/*	This object - at some point - should be smarter and store a cache of
	plotted graphs (moved from GraphPresenter) and additional infos...
*/
class PlotInfo : public QObject
{
	Q_OBJECT
public:
	void setPlot(QCustomPlot* p);
	const QColor& getBackgroundColor() const;
	void setBackgroundColor(const QColor& color);
	// axis ranges
	const std::pair<qreal, qreal> getXRange() const;
	void setXRange(const std::pair<qreal, qreal>& range);
	void setYRange(const QString& name, const std::pair<qreal, qreal>& range);
	void autoScaleY(const QString& name);
	void autoScaleX();
	void autoScaleAllY();
	// export
	void exportToPdf(const QString& fileName, qreal w = 0, qreal h = 0);
signals:
	void BackgroundColorChanged(const QColor&);
private:
	QColor backgroundColor;
	QCustomPlot* plot;
};
