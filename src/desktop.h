#ifndef swag_desktop_h
#define swag_desktop_h

#include "container.h"

class Desktop: public Container
{
public:
	Desktop();
	virtual Widget* Clone() const;
	void Set_child(Widget* widget);
	Widget* Get_child() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
	virtual void Resized();
	Widget* child;
};

#endif
