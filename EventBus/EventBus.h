#pragma once

#include "Event.h"
#include <map>
#include <queue>
using namespace std;

struct Event;

class EventBus
{
public:
	typedef int (*EventHandler)(Event* evt, void* ud);

public:
	EventBus();
	virtual ~EventBus();

	template<typename T, typename T1>
	int RegisterHandler(int (*handler)(T* evt, T1* ud), T1* ud)
	{
		return RegisterHandler(T::ID, (EventHandler)handler, (void*)ud);
	}

	template<typename T, typename T1>
	int UnregisterHandler(int(*handler)(T* evt, T1* ud), T1* ud)
	{
		return UnregisterHandler(T::ID, (EventHandler)handler, (void*)ud);
	}

	int RegisterHandler(int eventId, EventHandler handler, void* ud);

	int UnregisterHandler(int eventId, EventHandler handler, void* ud);

	int SendEvent(Event& event);

	int PushEvent(Event* event);

	int Dispatch();

	void PrintHandlers();

private:
	struct UserData
	{
		EventHandler handler;
		void* ud;
	};

	multimap<int, UserData*> m_handlerMap;

	queue<Event*> m_eventQueue;

};

