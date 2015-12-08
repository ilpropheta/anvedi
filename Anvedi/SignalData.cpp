#include "SignalData.h"
#include <limits>


void SignalData::clear()
{
	m_data.clear();
	emit DataCleared();
}

void SignalData::add(QString name, const Signal& data)
{
	m_data[name] = data;
	emit SignalAdded(data);
}

void SignalData::add(std::map<QString, Signal> data)
{
	m_data.insert(make_move_iterator(data.begin()), make_move_iterator(data.end()));
	emit DataAdded(m_data);
}

void SignalData::set(const QString& name, std::function<void(Signal&)> setter)
{
	auto& signal = getOrInsert(name);
	setter(signal);
	emit SignalChanged(signal);
}

void SignalData::setColor(const QString& name, const QColor& col)
{
	auto& signal = getOrInsert(name);
	signal.color = col;
	emit SignalColorChanged(signal);
}

void SignalData::setVisible(const QString& name, bool visible)
{
	auto& signal = getOrInsert(name);
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
	auto& inserted = m_data.emplace_hint(it.first, name, Signal::Create(name))->second;
	emit DataAdded(m_data);
	return inserted;
}

void SignalData::onSignals(std::function<void(const Signal&)> fun) const
{
	for (const auto& signal : m_data)
		fun(signal.second);
}

std::pair<qreal, size_t> SignalData::nearestDomainValueTo(qreal val)
{
	if (!m_data.empty())
	{
		const auto& x = m_data.begin()->second.x;
		const auto it = std::lower_bound(x.begin(), x.end(), val);
		return{ *it, std::distance(x.begin(), it) };
	}
	return{ std::numeric_limits<qreal>::quiet_NaN(), 0u };
}
