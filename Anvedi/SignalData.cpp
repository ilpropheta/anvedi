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
	auto it = m_data.equal_range(signal.name);
	if (it.first == it.second)
	{
		m_data.emplace_hint(it.first, signal.name, std::move(signal));
		emit DataAdded(m_data);
	}
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
	// Note: use this commented version if you just need to add new values
	/*if (domain)
	{
		const auto& domainSlice = data.at(domain->name);
		for (const auto& d : data)
			m_data.at(d.first).y << d.second;
		emit SignalAdded(domainSlice, data);
	}*/

	// the following piece of code supports re-sending old data (e.g. a recover) and sending new data of the past.
	// The (usual) precondition is the increasing order of the domain values.

	if (domain)
	{
		const auto& domainSlice = data.at(domain->name);

		if (domain->y.empty())
		{
			const auto& domainSlice = data.at(domain->name);
			for (const auto& d : data)
				m_data.at(d.first).y << d.second;
			emit SignalAdded(domainSlice, data);
			return;
		}

		// sugar to calculate the position of the iterator "to" 
		// in the domainSlice vector
		auto vectorIndex = [&](QVector<qreal>::const_iterator to){
			return std::distance(domainSlice.begin(), to);
		};

		// sugar to get the iterator in vec corresponding to the position of the iterator domainIt in domainSlice
		auto iteratorFromDomainIt = [&](const QVector<qreal>& vec, QVector<qreal>::const_iterator domainIt){
			return vec.begin() + vectorIndex(domainIt);
		};

		// support ds for making easy to add data to each signal
		// element i contains correspondence between ith-slice to its signal y vector
		std::vector<std::pair<const QVector<qreal>*, QVector<qreal>*>> sliceToSignalValues;
		sliceToSignalValues.reserve(data.size() - 1);
		for (const auto& d : data)
		{
			if (d.first != domain->name)
			{
				auto& vals = m_data.at(d.first).y;
				sliceToSignalValues.push_back(std::make_pair(&d.second, &vals));
				// preparing for growing
				vals.reserve(vals.size()+d.second.size());
			}
		}

		// the current domain values 
		// (can be already filled - e.g. you are comparing an RT with imported signals)
		auto& domainValues = domain->y;

		// for each added domain value 
		for (auto it = domainSlice.begin(); it != domainSlice.end(); ++it)
		{
			/* since domainSlice has to be ordered, if this condition holds
			   we can just add the remaining chunk of data
			   note: this condition always holds if you are just sending completely
			   new data */
			if (*it > domainValues.back()) // insert all
			{
				std::copy(it, domainSlice.end(), std::back_inserter(domainValues));
				for (auto& d : sliceToSignalValues)
				{
					std::copy(iteratorFromDomainIt(*d.first, it), d.first->end(), std::back_inserter(*d.second));
				}
				break; 
			}
			// we are receiving some new data of the past
			if (*it < domainValues.front()) 
			{
				domainValues.push_front(*it);
				for (auto& d : sliceToSignalValues)
				{
					d.second->push_front(d.first->at(vectorIndex(it)));
				}
			}
			// we are receiving data within the current domain
			else
			{
				auto eRange = std::equal_range(domainValues.begin(), domainValues.end(), *it);
				if (eRange.first == eRange.second) // new data within the domain
				{
					domainValues.insert(eRange.first, *it);
					for (auto& d : sliceToSignalValues)
					{
						const auto valueToAdd = *(iteratorFromDomainIt(*d.first, it));
						d.second->insert(d.second->begin() + std::distance(domainValues.begin(), eRange.first), valueToAdd);
					}
				}
				else // overwrite a possibly already added value within the domain
				{
					auto positionInCurrentDomain = std::distance(domainValues.begin(), eRange.first);
					for (auto& d : sliceToSignalValues)
					{
						const auto valueToAdd = *(iteratorFromDomainIt(*d.first, it));
						if (d.second->size() < positionInCurrentDomain) 
						{
							*(d.second->begin() + positionInCurrentDomain) = valueToAdd;
						}
						else
						{
							d.second->push_back(valueToAdd);
							// e.g. domain already filled with values but signals yet to fill
						}
					}
				}
			}
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

void SignalData::remove(const QString& what)
{
	auto it = m_data.find(what);
	if (it == m_data.end())
		throw std::exception("This signal does not exist");
	m_data.erase(it);
	emit SignalRemoved(what);
}

void SignalData::rename(const QString& what, const QString& newName)
{
	auto whatIt = m_data.find(what);
	auto newNameIt = m_data.find(newName);
	if (newNameIt != m_data.end())
		throw std::exception("This name already exists");
	if (whatIt == m_data.end())
		throw std::exception("This signal does not exist");
	
	auto signalToRename = whatIt->second;
	m_data.erase(whatIt);
	signalToRename.name = newName;
	auto inserted = m_data.insert(std::make_pair(newName, std::move(signalToRename)));
	emit SignalRenamed(what, inserted.first->second);
}
