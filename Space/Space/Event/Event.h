#pragma once
#include <functional>
#include <vector>
#include <memory>

template <typename T>
class EventHolder;

namespace
{
	template <typename T>
	struct EventImpl
	{
		EventImpl(std::function<void(T)> _function);

		std::function<void(T)> function;
		bool blocked;
		bool disconnected;
	};
}

template <typename T>
class Event
{
	friend class EventHolder<T>;
public:
	static EventHolder<T> connect(std::function<void(T)> function);
	static void send(T value);
private:
	Event() = delete;

	static std::vector<std::unique_ptr<EventImpl<T>>> m_events;
};

template <typename T>
std::vector<std::unique_ptr<EventImpl<T>>> Event<T>::m_events;

template <typename T>
class EventHolder
{
	friend Event<T>;
public:
	EventHolder();
	EventHolder(const EventHolder<T> &) = delete;
	EventHolder(EventHolder<T> && e);
	EventHolder<T> & operator=(const EventHolder<T> &) = delete;
	EventHolder<T> & operator=(EventHolder<T> && e);
	operator bool() const;
	bool isDisconnected() const;
	bool isBlocked() const;

	void blockEvent(bool block);
	void disconnect();

	~EventHolder();

private:
	EventHolder(EventImpl<T>* e);
	EventImpl<T>* m_eventImpl;
};

#include "Event.inl"