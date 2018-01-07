#include "EventBus.h"
#include "Event.h"
#include <stdio.h>


EventBus::EventBus()
{
}

EventBus::~EventBus()
{
}

int EventBus::RegisterHandler(int eventId, EventHandler handler, void * ud)
{
	int ret = UnregisterHandler(eventId, handler, ud);
	if (ret != 0)
	{
		printf("RegisterHandler eventId %d handler %p, ud %p repeated\n", eventId, handler, ud);
	}

	UserData * data = new UserData();
	data->handler = handler;
	data->ud = ud;
	m_handlerMap.insert(make_pair(eventId, data));

	return 0;
}

int EventBus::UnregisterHandler(int eventId, EventHandler handler, void * ud)
{
	for (multimap<int, UserData*>::iterator itr = m_handlerMap.lower_bound(eventId); itr != m_handlerMap.upper_bound(eventId); )
	{
		UserData* data = itr->second;
		if (data->handler == handler && data->ud == ud)
		{
			itr = m_handlerMap.erase(itr);
			delete data;
			return 0;
		}
		else
		{
			itr++;
		}
	}
	return 1;
}

int EventBus::SendEvent(Event& event)
{
	int eventId = event.GetId();
	for (multimap<int, UserData*>::iterator itr = m_handlerMap.lower_bound(eventId); itr != m_handlerMap.upper_bound(eventId); itr++)
	{
		itr->second->handler(&event, itr->second->ud);
	}
	return 0;
}

int EventBus::PushEvent(Event * event)
{
	m_eventQueue.push(event);
	return 0;
}

int EventBus::Dispatch()
{
	while (!m_eventQueue.empty())
	{
		Event * event = m_eventQueue.front();
		SendEvent(*event);
		m_eventQueue.pop();
		delete event;
	}
	return 0;
}

void EventBus::PrintHandlers()
{
	printf("PrintHandlers:\n");
	for (auto itr = m_handlerMap.begin(); itr != m_handlerMap.end(); itr++)
	{
		printf("eventId %d, handler %p, ud %p \n", (itr->first), itr->second->handler, itr->second->ud);
	}
}
