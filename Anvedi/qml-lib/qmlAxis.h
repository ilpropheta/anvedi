#pragma once
#include <QObject>
class qmlLabel;

class qmlAxis : public QObject
{
	Q_OBJECT
	Q_PROPERTY(qmlLabel* label READ getLabel WRITE setLabel)
	Q_PROPERTY(bool useDefault READ isDefault WRITE setUseDefault)
	
public:
	qmlLabel* getLabel() const;
	void setLabel(qmlLabel* l);

	bool isDefault() const;
	void setUseDefault(bool d);
private:
	qmlLabel* label = nullptr;
	bool default = true;
};