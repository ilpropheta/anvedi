#pragma once

#include <QObject>
#include <QColor>
#include <QVector>

class Signal : public QObject
{
	Q_PROPERTY(QString name READ getName WRITE setName);
	Q_PROPERTY(QColor color READ getColor WRITE setColor);
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible);
	Q_PROPERTY(QVector<qreal> x READ getX WRITE setX);
	Q_PROPERTY(QVector<qreal> y READ getY WRITE setY);
	Q_OBJECT
public:
	Signal(QString name, const QColor& col, QVector<qreal> x, QVector<qreal> y);

	const QString& getName() const;
	void setName(const QString& name);

	const QColor& getColor() const;
	void setColor(const QColor& color);

	bool isVisible() const;
	void setVisible(bool visible);

	const QVector<qreal>& getX() const;
	void setX(const QVector<qreal>& vals);

	const QVector<qreal>& getY() const;
	void setY(const QVector<qreal>& vals);
signals:
	void ColorChanged(const QColor&);
	void VisibilityChanged(bool);
	void DomainChanged(const QVector<qreal>&);
	void ValuesChanged(const QVector<qreal>&);
private:
	QString m_name;
	QColor m_color;
	bool m_visible;
	QVector<qreal> m_x;
	QVector<qreal> m_y;
};

