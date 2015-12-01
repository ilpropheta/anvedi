#pragma once

#include <QObject>
#include <QVariant>
#include <QColor>

class SignalData;

class SignalHandle : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
	Q_PROPERTY(QString color READ getColor WRITE setColor)
	Q_PROPERTY(QVariant values READ getValues WRITE setValues)
public:
	SignalHandle(QString name, SignalData& data);

	bool isVisible() const;
	void setVisible(bool visible);
	QString getColor() const;
	void setColor(const QString& color);
	QVariant getValues() const;
	void setValues(const QVariant& values);
private:
	QString signalName;
	SignalData& data;
};

