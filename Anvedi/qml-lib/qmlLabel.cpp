#include "qmlLabel.h"

const QString& qmlLabel::getText() const
{
	return text;
}

void qmlLabel::setText(const QString& txt)
{
	text = txt;
}

const QString& qmlLabel::getFont() const
{
	return font;
}

void qmlLabel::setFont(const QString& f)
{
	font = f;
}

const QColor& qmlLabel::getColor() const
{
	return color;
}

void qmlLabel::setColor(const QColor& c)
{
	color = c;
}
