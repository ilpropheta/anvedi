#pragma once

#include <QObject>
#include <map>
#include <functional>
#include "Signal.h"
#include <utility>

using DataMap = std::map < QString, Signal > ;

class SignalData : public QObject
{
	Q_OBJECT
public:
	void clear();
	void add(DataMap data);
	void add(QString name, const Signal& data);
	void set(const QString& name, std::function<void(Signal&)> setter);
	void setColor(const QString& name, const QColor& col);  // to move
	void setVisible(const QString& name, bool visible);  // to move
	const Signal& get(const QString& name) const;
	Signal& getOrInsert(const QString& name);
	void onSignals(std::function<void(const Signal&)> fun) const;
	std::pair<qreal, size_t> nearestDomainValueTo(qreal val);
signals:
	void DataAdded(const DataMap&);
	void SignalChanged(const Signal&);
	void SignalColorChanged(const Signal&); // to move
	void SignalVisibilityChanged(const Signal&); // to move
	void SignalAdded(const Signal&);
	void DataCleared();
private:
	DataMap m_data;
};

Q_DECLARE_METATYPE(DataMap)