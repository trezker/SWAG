#include "box.h"

void Box::Add(Widget* widget)
{
	if(widget==NULL)
		return;
	widgets.push_back(widget);
	widget->Set_parent(this);
}

const Widgets& Box::Get_widgets() const
{
	return widgets;
}

void Box::Resized()
{
	Organise();
}

void Box::Handle_event(const ALLEGRO_EVENT& event)
{
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		(*i)->Handle_event(event);
	}
}

void Box::Handle_child_resize()
{
	Organise();
	Child_resized();
}
