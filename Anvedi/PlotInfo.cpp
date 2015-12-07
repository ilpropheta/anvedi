#include "PlotInfo.h"


const QColor& PlotInfo::getBackgroundColor() const
{
	return backgroundColor;
}

void PlotInfo::setBackgroundColor(const QColor& color)
{
	backgroundColor = color;
	emit BackgroundColorChanged(color);
}
