#pragma once

#include "Event.h"
#include <algorithm>

template<typename T>
inline EventImpl<T>::EventImpl(std::function<void(T)> _function)
	: function(_function)
	, blocked(false)
	, disconnected(false)
{
}

template <typename T>
EventHolder<T> Event<T>::connect(std::function<void(T)> function)
{
	m_events.push_back(std::move(std::make_unique<EventImpl<T>>(function)));
	return EventHolder<T>(m_events.back().get());
}

template<typename T>
void Event<T>::send(T value)
{
	for (unsigned int i(0); i < m_events.size(); i++)
	{
		auto & e(m_events[i]);
		if (e->disconnected || e->blocked || !e->function)
			continue;
		e->function(value);
	}

	m_events.erase(std::remove_if(m_events.begin(), m_events.end(), [](const auto & v) {return v->disconnected; }), m_events.end());
}

template<typename T>
EventHolder<T>::EventHolder()
	: m_eventImpl(nullptr)
{
}

template<typename T>
EventHolder<T>::EventHolder(EventHolder<T> && e)
	: m_eventImpl(e.m_eventImpl)
{
	e.m_eventImpl = nullptr;
}

template<typename T>
EventHolder<T> & EventHolder<T>::operator=(EventHolder<T> && e)
{
	if(m_eventImpl != nullptr)
		m_eventImpl->disconnected = true;
	m_eventImpl = e.m_eventImpl;
	e.m_eventImpl = nullptr;
}

template<typename T>
EventHolder<T>::operator bool() const
{
	return !isDisconnected();
}

template<typename T>
bool EventHolder<T>::isDisconnected() const
{
	if (m_eventImpl == nullptr)
		return true;
	return m_eventImpl->disconnected;
}

template<typename T>
bool EventHolder<T>::isBlocked() const
{
	if (m_eventImpl == nullptr)
		return false;
	return m_eventImpl->blocked;
}

template<typename T>
void EventHolder<T>::blockEvent(bool block)
{
	if (m_eventImpl == nullptr)
		return;
	m_eventImpl->blocked = block;
}

template<typename T>
void EventHolder<T>::disconnect()
{
	if (m_eventImpl == nullptr)
		return;
	m_eventImpl->disconnected = true;
	m_eventImpl = nullptr;
}

template<typename T>
EventHolder<T>::~EventHolder()
{
	disconnect();
}

template<typename T>
EventHolder<T>::EventHolder(EventImpl<T> * e)
	: m_eventImpl(e)
{
}
