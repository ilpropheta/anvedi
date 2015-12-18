#pragma warning (disable:4996) // due to QVector operator==

#include "Signal.h"

bool Signal::operator==(const Signal& other) const
{
	return std::tie(name, color, visible, y) ==
		std::tie(other.name, other.color, other.visible, other.y);
}
