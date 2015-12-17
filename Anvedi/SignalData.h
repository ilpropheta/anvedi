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
	// getters
	const Signal& get(const QString& name) const;
	const Signal* getDomain() const;
	// setters
	void setAsDomain(const QString& name);
	void setColor(const QString& name, const QColor& col);
	void setVisible(const QString& name, bool visible);
	void setValues(const QString& name, QVector<qreal> vec);
	// visitor
	void onSignals(std::function<void(const Signal&)> fun) const;
	// utils
	std::pair<qreal, size_t> domainLowerBound(qreal val) const;
signals:
	void DataAdded(const DataMap&);
	void SignalValuesChanged(const Signal&);
	void SignalColorChanged(const Signal&);
	void SignalVisibilityChanged(const Signal&);
	void DataCleared();
	void DomainChanged(const Signal&);
private:
	Signal& getOrInsert(const QString& name);

	DataMap m_data;
	Signal* domain = nullptr;
};

Q_DECLARE_METATYPE(DataMap)