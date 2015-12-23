#pragma once
#include <QObject>
#include <QColor>
#include <QVector>
#include <QVariant>

class qmlTick : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString font READ getFont WRITE setFont)
	Q_PROPERTY(QVariantList vector READ getVector WRITE setVector)
	Q_PROPERTY(QVariantList labels READ getLabels WRITE setLabels)
public:
	const QString& getFont() const;
	void setFont(const QString& f);
	QVariantList getVector() const;
	void setVector(const QVariantList& f);
	QVariantList getLabels() const;
	void setLabels(const QVariantList& f);
	
	const QVector<qreal>& getTickVector() const;
	const QVector<QString>& getTickLabels() const;
private:
	QString font;
	QVector<qreal> vector;
	QVector<QString> labels;
};