#include "PlotHandle.h"
#include "PlotInfo.h"
#include <QPoint>

PlotHandle::PlotHandle(PlotInfo& plot)
	: plot(plot)
{
}

QString PlotHandle::getBackground() const
{
	return plot.getBackgroundColor().name();
}

void PlotHandle::setBackground(const QString& color)
{
	plot.setBackgroundColor(color);
}

QVariant PlotHandle::getXRange() const
{
	const auto xRange = plot.getXRange();
	QVariantList vals; 
	vals.push_back(xRange.first);
	vals.push_back(xRange.second);
	return vals;
}

void PlotHandle::setXRange(const QVariant& val) const
{
	const auto vList = val.toList();
	plot.setXRange({ vList.front().toReal(), vList.back().toReal() });
}
