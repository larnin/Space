#pragma once

#include "Event.h"
#include <algorithm>
#include <iostream>
#include <typeindex>
		

template<typename T>
EventImpl<T>::EventImpl(const std::function<void(const T &)> & _function)
	: function(_function)
	, blocked(false)
	, disconnected(false)
{
}

template <typename T>
EventHolder<T> Event<T>::connect(const std::function<void(const T &)> & function)
{
	std::cout << "C " << std::type_index(typeid(T)).hash_code() << " " << &m_events << std::endl;
	//std::cout << __FUNCSIG__ << std::endl;
	m_events.push_back(std::make_unique<EventImpl<T>>(function));
	return EventHolder<T>(m_events.back().get());
}

template<typename T>
void Event<T>::send(const T & value)
{
	std::cout << "S " << std::type_index(typeid(T)).hash_code() << " " << &m_events << std::endl;
	//std::cout << __FUNCSIG__ << std::endl;
	for (auto & e : m_events)
	{
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
