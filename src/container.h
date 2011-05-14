#ifndef swag_container_h
#define swag_container_h

#include "widget.h"

class Container: public Widget
{
public:
	virtual void Handle_child_resize();
	virtual bool Add_child(Widget* c) = 0;
	virtual void Remove_child(Widget* c) = 0;
	virtual Widgets Get_children() const = 0;
};

#endif
