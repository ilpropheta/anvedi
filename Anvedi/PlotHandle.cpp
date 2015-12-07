#include "PlotHandle.h"
#include "PlotInfo.h"

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
