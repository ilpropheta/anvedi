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
		for (auto&& elem : data)
		{
			auto it = m_data.equal_range(elem.first);
			if (it.first != it.second)
				it.first->second = std::move(elem.second);
			else
			{
				m_data.insert(it.first, std::move(elem));
			}
		}
		emit DataAdded(m_data);
	}
}

void SignalData::addEmptyIfNotExists(const QString& name)
{
	getOrInsert(name);
}

void SignalData::setValues(const QString& name, QVector<qreal> vec)
{
	auto& signal = m_data.at(name);
	signal.y = std::move(vec);
	emit SignalValuesChanged(signal);
	if (&signal == domain)
		emit DomainChanged(signal);
}

void SignalData::setColor(const QString& name, const QColor& col)
{
	auto& signal = m_data.at(name);
	signal.color = col;
	emit SignalColorChanged(signal);
}

void SignalData::setVisible(const QString& name, bool visible)
{
	auto& signal = m_data.at(name);
	signal.visible = visible;
	emit SignalVisibilityChanged(signal);
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