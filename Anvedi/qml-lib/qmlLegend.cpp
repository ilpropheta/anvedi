#include "qmlLegend.h"

QString qmlLegend::getFontString() const
{
	return font.toString();
}

void qmlLegend::setFontString(const QString& f)
{
	font.fromString(f);
}

const QFont& qmlLegend::getFont() const
{
	return font;
}
