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

inline std::pair<qreal, qreal> ToRange(const QVariant& val)
{
	const auto vList = val.toList();
	return { vList.front().toReal(), vList.back().toReal() };
}

void PlotHandle::setXRange(const QVariant& val) const
{
	plot.setXRange(ToRange(val));
}

void PlotHandle::setYRange(const QString& signalName, const QVariant& val)
{
	plot.setYRange(signalName, ToRange(val));
}

void PlotHandle::autoRangeY(const QString& name)
{
	plot.autoScaleY(name);
}

void PlotHandle::autoRangeX()
{
	plot.autoScaleX();
}

void PlotHandle::autoRangeAllY()
{
	plot.autoScaleAllY();
}

void PlotHandle::savePdf(const QString& fileName)
{
	plot.exportToPdf(fileName);
}

void PlotHandle::savePdf(const QString& fileName, qreal w, qreal h)
{
	plot.exportToPdf(fileName, w, h);
}