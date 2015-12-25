#pragma once
#include <QObject>
#include <QVariant>

class PlotInfo;

class PlotHandle : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString background READ getBackground WRITE setBackground)
	Q_PROPERTY(QVariant xRange READ getXRange WRITE setXRange)
public:
	PlotHandle(PlotInfo& plot);
	
	QString getBackground() const;
	void setBackground(const QString& color);
	QVariant getXRange() const;
	void setXRange(const QVariant& val) const;
public slots:
	void setYRange(const QString& signalName, const QVariant& val);
	void autoRangeAllY();
	void savePdf(const QString& fileName);
	void savePdf(const QString& fileName, qreal w, qreal h);
private:
	PlotInfo& plot;
};

