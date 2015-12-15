#pragma once
#include <QObject>

class qmlAxis : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString label READ getLabel WRITE setLabel)
	Q_PROPERTY(bool useDefault READ isDefault WRITE setUseDefault)
public:
	const QString& getLabel() const;
	void setLabel(const QString& l);

	bool isDefault() const;
	void setUseDefault(bool d);
private:
	QString label;
	bool default = true;
};