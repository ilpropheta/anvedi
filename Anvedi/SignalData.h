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
	void addIfNotExists(Signal signal);
	void clear();
	// getters
	const Signal& get(const QString& name) const;
	const Signal* getDomain() const;
	// setters
	void setAsDomain(const QString& name);
	void setValues(const QString& name, QVector<qreal> vec);
	void addValues(const std::map<QString, QVector<qreal>>& data);
	void setSignalGraphic(const QString& name, SignalGraphic info);
	void setColor(const QString& name, const QColor& col);
	void setVisible(const QString& name, bool visible);
	void setRange(const QString& name, double lo, double up);
	void setAutoRange(const QString& name);
	void setAutoRangeAll();
	void setTicks(const QString& name, QVector<qreal> ticks);
	void setTickLabels(const QString& name, QVector<QString> ticks);
	// visitor
	void onSignals(std::function<void(const Signal&)> fun) const;
	// utils
	std::pair<qreal, size_t> domainLowerBound(qreal val) const;
	qreal domainNextValue(qreal refValue) const;
	qreal domainPrevValue(qreal refValue) const;
signals:
	void DataAdded(const DataMap&);
	void SignalValuesChanged(const Signal&);
	void SignalValuesAdded(const Signal&, const QVector<qreal>&, const QVector<qreal>&);
	void SignalAdded(const QVector<qreal>&, const std::map<QString, QVector<qreal>>&);
	void SignalGraphicChanged(const Signal&);
	void SignalColorChanged(const Signal&);
	void SignalVisibilityChanged(const Signal&);
	void SignalRangeChanged(const Signal&);
	void SignalTicksChanged(const Signal&);
	void DataCleared();
	void DomainChanged(const Signal&);
private:
	Signal& getOrInsert(const QString& name);

	DataMap m_data;
	Signal* domain = nullptr;
};

Q_DECLARE_METATYPE(DataMap)