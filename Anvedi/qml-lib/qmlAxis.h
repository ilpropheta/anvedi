#pragma once
#include <QObject>

class qmlLabel;
class qmlTick;

class qmlAxis : public QObject
{
	Q_OBJECT
	Q_PROPERTY(qmlLabel* label READ getLabel WRITE setLabel)
	Q_PROPERTY(qmlTick* tick READ getTick WRITE setTick)
	Q_PROPERTY(bool useDefault READ isDefault WRITE setUseDefault)
	Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
public:
	qmlLabel* getLabel() const;
	void setLabel(qmlLabel* l);
	qmlTick* getTick() const;
	void setTick(qmlTick* t);
	bool isDefault() const;
	void setUseDefault(bool d);
	bool isVisible() const;
	void setVisible(bool d);
private:
	qmlLabel* label = nullptr;
	qmlTick* tick = nullptr;
	bool default = true;
	bool visible = true;
};