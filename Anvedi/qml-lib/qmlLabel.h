#pragma once
#include <QObject>
#include <QColor>

class qmlLabel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString text READ getText WRITE setText)
	Q_PROPERTY(QString font READ getFont WRITE setFont)
	Q_PROPERTY(QColor color READ getColor WRITE setColor)
public:
	const QString& getText() const;
	void setText(const QString& txt);
	const QString& getFont() const;
	void setFont(const QString& f);
	const QColor& getColor() const;
	void setColor(const QColor& c);
private:
	QString text;
	QString font;
	QColor color;
};