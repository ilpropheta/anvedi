#include "qmlScatterStyle.h"

QCPScatterStyle::ScatterShape qmlScatterStyle::getType() const
{
	return style.shape();
}

void qmlScatterStyle::setType(QCPScatterStyle::ScatterShape t)
{
	style.setShape(t);
}

qmlPen* qmlScatterStyle::getPen() const
{
	return nullptr;
}

void qmlScatterStyle::setPen(qmlPen* p)
{
	style.setPen(p->getPen());
}

qreal qmlScatterStyle::getSize() const
{
	return style.size();
}

void qmlScatterStyle::setSize(qreal w)
{
	style.setSize(w);
}

QColor qmlScatterStyle::getBrush() const
{
	return style.brush().color();
}

void qmlScatterStyle::setBrush(const QColor& c)
{
	style.setBrush(c);
}

const QCPScatterStyle& qmlScatterStyle::getStyle() const
{
	return style;
}
