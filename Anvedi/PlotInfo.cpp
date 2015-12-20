#include "PlotInfo.h"
#include "qcustomplot.h"

void PlotInfo::setPlot(QCustomPlot* p)
{
	plot = p;
}

const QColor& PlotInfo::getBackgroundColor() const
{
	return backgroundColor;
}

void PlotInfo::setBackgroundColor(const QColor& color)
{
	backgroundColor = color;
	emit BackgroundColorChanged(color);
}

void PlotInfo::setXRange(const std::pair<qreal, qreal>& range)
{
	plot->xAxis->setRange(range.first, range.second);
}

const std::pair<qreal, qreal> PlotInfo::getXRange() const
{
	const auto range = plot->xAxis->range();
	return { range.lower, range.upper };
}