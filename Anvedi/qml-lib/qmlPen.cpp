#include "qmlPen.h"

QColor qmlPen::getColor() const
{
	return pen.color();
}

void qmlPen::setColor(const QColor& c)
{
	pen.setColor(c);
}

QColor qmlPen::getBrush() const
{
	return pen.brush().color();
}

void qmlPen::setBrush(const QColor& c)
{
	pen.setBrush(c);
}

qreal qmlPen::getWidth() const
{
	return pen.width();
}

void qmlPen::setWidth(qreal w)
{
	pen.setWidthF(w);
}

const QPen& qmlPen::getPen() const
{
	return pen;
}
