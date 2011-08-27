#include "event_queue.h"
#include <iostream>

Event::Event(Widget* s, const std::string& t)
:source(s)
,type(t)
{
}

Event* Event::Clone() const
{
	return new Event(*this);
}

Event_queue::~Event_queue()
{
	while(!events.empty())
	{
		delete events.front();
		events.pop();
	}
}

void Event_queue::Push(const Event& e)
{
	events.push(e.Clone());
}

void Event_queue::Pop()
{
	if(!events.empty())
	{
		delete events.front();
		events.pop();
	}
}

bool Event_queue::Empty() const
{
	return events.empty();
}

const Event& Event_queue::Front() const
{
	return *events.front();
}

bool Event::operator< (const Event& o) const {
	if(source>o.source) {
		return false;
	} else if(source<o.source) {
		return true;
	}
	if(type<o.type) {
		return true;
	}
	return false;
}
