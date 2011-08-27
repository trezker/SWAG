#ifndef swag_event_queue_h
#define swag_event_queue_h

#include <queue>
#include <string>

class Widget;

class Event
{
public:
	Event(Widget* source, const std::string& type);
	Widget* source;
	std::string type;
	virtual Event* Clone() const;
	bool operator< (const Event& o) const;
};

class Event_queue
{
public:
	~Event_queue();
	void Push(const Event& e);
	void Pop();
	bool Empty() const;
	const Event& Front() const;
private:
	typedef std::queue<Event*> Events;
	Events events;
};

#endif
