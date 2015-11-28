#pragma once

#include <xutility>
#include <list>
#include "qshell_global.h"

template <typename Command>
class HistoryRecorder 
{
public:

	HistoryRecorder() : m_current(history.end()) {}

	bool IsEmpty() 
	{
		return history.empty();
	}

	void Add(const Command& command) 
	{
		history.push_front(command);
		m_current = history.end();
	}

	void Add(Command&& command) 
	{
		history.push_front(std::move(command));
		m_current = history.end();
	}

	Command Next() 
	{
		if (history.empty())
			return Command();

		if (m_current == history.end()) 
		{
			m_current = history.begin();
			return *m_current;
		}

		auto next = m_current;
		++next;
		return next==history.end() ? *m_current : *(++m_current);
	}

	Command Previous() 
	{
		if (history.empty() || m_current == history.end())
			return Command();

		return m_current==history.begin() ? *m_current : *(--m_current);
	}

	void Clear() 
	{
		history.clear();
		m_current = history.end();
	}

private:
	std::list<Command>						history;
	typename std::list<Command>::iterator	m_current;
};