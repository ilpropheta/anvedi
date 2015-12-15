#include "qmlAxis.h"

const QString& qmlAxis::getLabel() const
{
	return label;
}

void qmlAxis::setLabel(const QString& l)
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
