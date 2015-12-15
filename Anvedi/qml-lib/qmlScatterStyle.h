#pragma once
#include <QObject>
#include "qcustomplot.h"
#include "qmlPen.h"

class qmlScatterStyle : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QCPScatterStyle::ScatterShape type READ getType WRITE setType)
	Q_PROPERTY(qreal size READ getSize WRITE setSize)
	Q_PROPERTY(qmlPen* pen READ getPen WRITE setPen)
	Q_PROPERTY(QColor brush READ getBrush WRITE setBrush)
public:
	QCPScatterStyle::ScatterShape getType() const;
	void setType(QCPScatterStyle::ScatterShape);

	qmlPen* getPen() const;
	void setPen(qmlPen* p);

	qreal getSize() const;
	void setSize(qreal w);

	QColor getBrush() const;
	void setBrush(const QColor& c);

	const QCPScatterStyle& getStyle() const;
private:
	QCPScatterStyle style;
};