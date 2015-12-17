#include "SignalData.h"
#include <limits>
#include <numeric>

void SignalData::clear()
{
	m_data.clear();
	domain = nullptr;
	emit DataCleared();
}

void SignalData::add(DataMap data)
{
	if (!data.empty())
	{
		m_data.insert(make_move_iterator(data.begin()), make_move_iterator(data.end()));
		emit DataAdded(m_data);
	}
}

void SignalData::addEmptyIfNotExists(const QString& name)
{
	getOrInsert(name);
}

void SignalData::set(const QString& name, std::function<void(Signal&)> setter)
{
	auto& signal = get(name);
	setter(signal);
	emit SignalChanged(signal);
	if (&signal == domain)
		emit DomainChanged(signal);
}

void SignalData::setColor(const QString& name, const QColor& col)
{
	auto& signal = get(name);
	signal.color = col;
	emit SignalColorChanged(signal);
}

void SignalData::setVisible(const QString& name, bool visible)
{
	auto& signal = get(name);
	signal.visible = visible;
	emit SignalVisibilityChanged(signal);
}

const Signal& SignalData::get(const QString& name) const
{
	return m_data.at(name);
}

Signal& SignalData::get(const QString& name)
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
	if (domain)
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
	domain = &get(name);
	emit DomainChanged(*domain);
}