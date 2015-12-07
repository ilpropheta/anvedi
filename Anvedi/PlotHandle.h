#pragma once
#include <QObject>

class PlotInfo;

class PlotHandle : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString background READ getBackground WRITE setBackground)
public:
	PlotHandle(PlotInfo& plot);
	
	QString getBackground() const;
	void setBackground(const QString& color);
private:
	PlotInfo& plot;
};

