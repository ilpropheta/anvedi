#pragma once
#include <QColor>

bool close(const QColor& c1, const QColor& c2);

inline QColor invert(const QColor& col)
{
	return{ 255 - col.red(), 255 - col.green(), 255 - col.blue() };
}