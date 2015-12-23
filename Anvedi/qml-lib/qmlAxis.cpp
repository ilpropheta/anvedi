#include "qmlAxis.h"
#include "qmlLabel.h"
#include "qmlTick.h"

qmlLabel* qmlAxis::getLabel() const
{
	return label;
}

void qmlAxis::setLabel(qmlLabel* l)
{
	label = l;
}

qmlTick* qmlAxis::getTick() const
{
	return tick;
}

void qmlAxis::setTick(qmlTick* t)
{
	tick = t;
}

bool qmlAxis::isDefault() const
{
	return default;
}

void qmlAxis::setUseDefault(bool d)
{
	default = d;
}


