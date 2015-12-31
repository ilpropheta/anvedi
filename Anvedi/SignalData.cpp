#include "SignalData.h"
#include <limits>
#include <numeric>

void SignalData::clear()
{
	m_data.clear();
	domain = nullptr;
	emit DataCleared();
}

void AutoRange(Signal& signal)
{
	const auto minMax = std::minmax_element(std::begin(signal.y), std::end(signal.y));
	signal.graphic.rangeLower = *minMax.first;
	signal.graphic.rangeUpper = *minMax.second;
}

void AutoRangeIfInvalid(Signal& signal)
{
	if (signal.graphic.rangeLower == signal.graphic.rangeUpper)
	{
		AutoRange(signal);
	}
}

void SignalData::add(DataMap data)
{
	if (!data.empty())
	{
		for (auto&& elem : data)
		{
			auto it = m_data.equal_range(elem.first);
			if (it.first != it.second)
			{
				it.first->second = std::move(elem.second);
				AutoRangeIfInvalid(it.first->second);
			}
			else
			{
				auto inserted = m_data.insert(it.first, std::move(elem));
				AutoRangeIfInvalid(inserted->second);
			}
		}
		emit DataAdded(m_data);
	}
}

void SignalData::addEmptyIfNotExists(const QString& name)
{
	getOrInsert(name);
}

void SignalData::addIfNotExists(Signal signal)
{
	getOrInsert(signal.name) = std::move(signal);
}

void SignalData::setValues(const QString& name, QVector<qreal> vec)
{
	auto& signal = m_data.at(name);
	signal.y = std::move(vec);
	AutoRange(signal);
	emit SignalValuesChanged(signal);
	if (&signal == domain)
		emit DomainChanged(signal);
}

void SignalData::setRange(const QString& name, double lo, double up)
{
	auto& signal = m_data.at(name);
	signal.graphic.rangeLower = lo;
	signal.graphic.rangeUpper = up;
	emit SignalRangeChanged(signal);
}

void SignalData::setAutoRange(const QString& name)
{
	auto& signal = m_data.at(name);
	AutoRange(signal);
	SignalRangeChanged(signal);
}

void SignalData::addValues(const std::map<QString, QVector<qreal>>& data)
{
	/* Note: this version does not support recovering (e.g. resending data from
	   the past, or sending new data of the past). I have a version of this function
	   properly supporting recovering. On the QCustomPlot side no problem because it
	   uses a map (actually it was using .insertMulti and then I replace it with .insert).
	*/
	if (domain)
	{
		const auto& domainSlice = data.at(domain->name);
		for (const auto& d : data)
		{
			auto& signal = m_data.at(d.first);
			signal.y << d.second;
			AutoRange(signal);
		}
		emit SignalAdded(domainSlice, data);
	}
}

void SignalData::setSignalGraphic(const QString& name, SignalGraphic info)
{
	auto& signal = m_data.at(name);
	signal.graphic = std::move(info);
	emit SignalGraphicChanged(signal);
}

void SignalData::setColor(const QString& name, const QColor& col)
{
	auto& signal = m_data.at(name);
	signal.graphic.color = col;
	emit SignalColorChanged(signal);
}

void SignalData::setVisible(const QString& name, bool visible)
{
	auto& signal = m_data.at(name);
	signal.graphic.visible = visible;
	emit SignalVisibilityChanged(signal);
}

void SignalData::setTicks(const QString& name, QVector<qreal> ticks)
{
	auto& signal = m_data.at(name);
	signal.graphic.ticks = std::move(ticks);
	emit SignalTicksChanged(signal);
}

void SignalData::setTickLabels(const QString& name, QVector<QString> ticks)
{
	auto& signal = m_data.at(name);
	signal.graphic.tickLabels = std::move(ticks);
	emit SignalTicksChanged(signal);
}

const Signal& SignalData::get(const QString& name) const
{
	return m_data.at(name);
}

Signal& SignalData::getOrInsert(const QString& name)
{
	auto it = m_data.equal_range(name);
	if (it.first != it.second)
		return it.first->second;
	auto& inserted = m_data.emplace_hint(it.first, name, Signal{ name })->second;
	emit DataAdded(m_data);
	return inserted;
}

void SignalData::onSignals(std::function<void(const Signal&)> fun) const
{
	for (const auto& signal : m_data)
		fun(signal.second);
}

std::pair<qreal, size_t> SignalData::domainLowerBound(qreal val) const
{
	if (domain && !domain->y.empty())
	{
		const auto& x = domain->y;
		const auto eRange = std::equal_range(x.begin(), x.end(), val);
		if (eRange.first == x.end()) // saturate right
		{
			return{ x.back(), x.size() - 1 };
		}
		if (eRange.first == x.begin()) // saturate left
		{
			return{ x.front(), 0u };
		}
		return{ *eRange.first, std::distance(x.begin(), eRange.first) };
	}
	return{ std::numeric_limits<qreal>::quiet_NaN(), 0u };
}

const Signal* SignalData::getDomain() const
{
	return domain;
}

void SignalData::setAsDomain(const QString& name)
{
	domain = &m_data.at(name);
	emit DomainChanged(*domain);
}

qreal SignalData::domainNextValue(qreal refValue) const
{
	if (domain && !domain->y.empty())
	{
		const auto& x = domain->y;
		const auto upperBound = std::upper_bound(x.begin(), x.end(), refValue);
		return upperBound != x.end() ? *upperBound : x.back();
	}
	return std::numeric_limits<qreal>::quiet_NaN();
}

qreal SignalData::domainPrevValue(qreal refValue) const
{
	if (domain && !domain->y.empty())
	{
		const auto& x = domain->y;
		const auto lowerBound = std::lower_bound(x.begin(), x.end(), refValue);
		return lowerBound != x.begin() ? *(lowerBound - 1) : x.front();
	}
	return std::numeric_limits<qreal>::quiet_NaN();
}

void SignalData::setAutoRangeAll()
{
	for (auto& signal : m_data)
	{
		AutoRange(signal.second);
		emit SignalRangeChanged(signal.second);
	}
}
