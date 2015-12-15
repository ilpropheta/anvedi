#pragma once
#include <QObject>
#include <QPen>

class qmlPen : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ getColor WRITE setColor)
	Q_PROPERTY(QColor brush READ getBrush WRITE setBrush)
	Q_PROPERTY(qreal width READ getWidth WRITE setWidth)
public:
	QColor getColor() const;
	void setColor(const QColor& c);
	QColor getBrush() const;
	void setBrush(const QColor& c);
	qreal getWidth() const;
	void setWidth(qreal w);

	const QPen& getPen() const;
private:
	QPen pen;
};