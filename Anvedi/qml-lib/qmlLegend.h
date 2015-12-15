#pragma once
#include <QObject>
#include "qcustomplot.h"

class qmlLegend : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString font READ getFontString WRITE setFontString)
public:
	QString getFontString() const;
	void setFontString(const QString& f);

	const QFont& getFont() const;
private:
	QFont font;
};