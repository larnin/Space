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
		EventImpl(const std::function<void(const T &)> & _function);

		std::function<void(const T &)> function;
		bool blocked;
		bool disconnected;
	};
}

template <typename T>
class Event
{
	friend class EventHolder<T>;
public:
	Event() = default;

	EventHolder<T> connectInstance(const std::function<void(const T &)> & function);
	void sendInstance(const T & value);

	static EventHolder<T> connect(const std::function<void(const T &)> & function);
	static void send(const T & value);
	static Event<T> & instance();

private:
	std::vector<std::unique_ptr<EventImpl<T>>> m_events;
};

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