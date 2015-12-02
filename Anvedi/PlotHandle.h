#pragma once
#include <QObject>

class PlotHandle : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString background READ getBackground WRITE setBackground)
public:
	PlotHandle();

	QString getBackground() const;
	void setBackground(const QString& color);
private:
	
};

