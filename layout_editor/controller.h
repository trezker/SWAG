#ifndef controller_h
#define controller_h

#include <map>
#include "swag.h"

class Controller {
public:
	virtual ~Controller();
	//Load layout and setup the event handles
	virtual bool Load(Skin& skin) = 0;
	virtual Widget* Get_root() = 0;
	void Handle_event(const Event& event);
	virtual void Handle_event(const Ustring& event_handle) = 0;
	virtual void Synchronize_values() = 0;
protected:
	typedef std::map<Event, Ustring> Events;
	Events events;
	Layout layout;
};

#endif
