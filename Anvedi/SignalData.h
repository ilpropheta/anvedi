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
	void add(DataMap data);
	void addEmptyIfNotExists(const QString& name);
	void clear();
	
	const Signal& get(const QString& name) const;
	const Signal* getDomain() const;
	
	void set(const QString& name, std::function<void(Signal&)> setter);
	void setAsDomain(const QString& name);
	void setColor(const QString& name, const QColor& col);
	void setVisible(const QString& name, bool visible);
	
	void onSignals(std::function<void(const Signal&)> fun) const;
	
	std::pair<qreal, size_t> nearestDomainValueTo(qreal val) const;
signals:
	void DataAdded(const DataMap&);
	void SignalChanged(const Signal&);
	void SignalColorChanged(const Signal&);
	void SignalVisibilityChanged(const Signal&);
	void DataCleared();
	void DomainChanged(const Signal&);
private:
	Signal& get(const QString& name);
	Signal& getOrInsert(const QString& name);

	DataMap m_data;
	Signal* domain = nullptr;
};

Q_DECLARE_METATYPE(DataMap)