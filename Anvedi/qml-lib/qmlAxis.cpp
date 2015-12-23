#include "qmlAxis.h"
#include "qmlLabel.h"

qmlLabel* qmlAxis::getLabel() const
{
	return label;
}

void qmlAxis::setLabel(qmlLabel* l)
{
	label = l;
}

bool qmlAxis::isDefault() const
{
	return default;
}

void qmlAxis::setUseDefault(bool d)
{
	default = d;
}
