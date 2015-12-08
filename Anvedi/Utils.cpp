#include "Utils.h"
#include <complex>

bool close(const QColor& c1, const QColor& c2)
{
	return std::sqrt(
		std::norm(c1.red() - c2.red()) +
		std::norm(c1.green() - c2.green()) +
		std::norm(c1.blue() - c2.blue())) < 10;
}
