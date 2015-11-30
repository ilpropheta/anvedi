#include "Signal.h"

using namespace std;

Signal::Signal(QString name, const QColor& col, QVector<qreal> x, QVector<qreal> y)
	: m_name(move(name)), m_color(col), m_x(move(x)), m_y(move(y))
{

}

const QString& Signal::getName() const
{
	return m_name;
}

void Signal::setName(const QString& name)
{
	m_name = name;
}

const QColor& Signal::getColor() const
{
	return m_color;
}

void Signal::setColor(const QColor& color)
{
	m_color = color;
	emit ColorChanged(m_color);
}

bool Signal::isVisible() const
{
	return m_visible;
}

void Signal::setVisible(bool visible)
{
	m_visible = visible;
	emit VisibilityChanged(m_visible);
}

const QVector<qreal>& Signal::getX() const
{
	return m_x;
}

void Signal::setX(const QVector<qreal>& vals)
{
	m_x = vals;
	emit DomainChanged(m_x);
}

const QVector<qreal>& Signal::getY() const
{
	return m_y;
}

void Signal::setY(const QVector<qreal>& vals)
{
	m_y = vals;
	emit ValuesChanged(m_y);
}
