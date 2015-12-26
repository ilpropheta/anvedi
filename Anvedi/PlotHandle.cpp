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
	if ((val.type() == QVariant::String) && val.toString() == "auto")
		plot.autoScaleX();
	else
		plot.setXRange(ToRange(val));
}

void PlotHandle::setYRange(const QString& signalName, const QVariant& val)
{
	if ((val.type() == QVariant::String) && val.toString() == "auto")
		plot.autoScaleY(signalName);
	else
		plot.setYRange(signalName, ToRange(val));
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

int PlotHandle::getRTPageSize() const
{
	return plot.getRealTimePageSize();
}

void PlotHandle::setRTPageSize(int ps)
{
	plot.setRealTimePageSize(ps);
}
