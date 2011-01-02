#include "desktop.h"
#include <iostream>

Desktop::Desktop()
:child(NULL)
{
}

Widget* Desktop::Clone() const
{
	return new Desktop(*this);
}

void Desktop::Set_child(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	if(child)
		child->Set_parent(NULL);
	child = widget;
}

Widget* Desktop::Get_child() const
{
	return child;
}

void Desktop::Handle_event(const ALLEGRO_EVENT& event)
{
	if (ALLEGRO_EVENT_DISPLAY_RESIZE == event.type)
	{
		Set_size(Vector2(event.display.width, event.display.height));
	}

	if(child)
	{
		child->Handle_event(event);
	}
}

void Desktop::Resized()
{
	if(child)
	{
		child->Set_size(Get_size());
	}
}
