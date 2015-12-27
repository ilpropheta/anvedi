#pragma warning (disable:4996) // due to QVector operator==

#include "Signal.h"

bool Signal::operator==(const Signal& other) const
{
	return std::tie(name, y, graphic) ==
		std::tie(other.name, other.y, other.graphic);
}

bool SignalGraphic::operator==(const SignalGraphic& other) const
{
	return std::tie(color, visible, rangeLower, rangeUpper, ticks, tickLabels) ==
		std::tie(other.color, other.visible, other.rangeLower, other.rangeUpper, other.ticks, other.tickLabels);
}
