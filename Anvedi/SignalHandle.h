#pragma once

#include <QObject>
#include <QVariant>
#include <QColor>

class SignalData;

class SignalHandle : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
	Q_PROPERTY(QString name READ getName WRITE setName)
	Q_PROPERTY(QString color READ getColor WRITE setColor)
	Q_PROPERTY(QVariant values READ getValues WRITE setValues)
	Q_PROPERTY(QVariant range READ getRange WRITE setRange)
	Q_PROPERTY(QVariant ticks READ getTicks WRITE setTicks)
public:
	SignalHandle(QString name, SignalData& data);

	// props
	QString getName() const;
	void setName(const QString& name);
	bool isVisible() const;
	void setVisible(bool visible);
	QString getColor() const;
	void setColor(const QString& color);
	QVariant getValues() const;
	void setValues(const QVariant& values);
	QVariant getRange() const;
	void setRange(const QVariant& values);
	QVariant getTicks() const;
	void setTicks(const QVariant& values);

	void SetThisAsDomain();
public slots:	
	QString toString() const;
private:
	QString signalName;
	SignalData& data;
};

