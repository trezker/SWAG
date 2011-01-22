#include "box.h"
#include <algorithm>

void Box::Add(Widget* widget)
{
	if(widget==NULL)
		return;
	widgets.push_back(widget);
	widget->Set_parent(this);
	Organise();
	Child_resized();
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

bool Box::Add_child(Widget* c)
{
	Add(c);
	return true;
}

void Box::Remove_child(Widget* c)
{
	Widgets::iterator i = std::find(widgets.begin(), widgets.end(), c);
	if(i != widgets.end())
	{
		widgets.erase(i);
	}
	Organise();
}

const std::string& Box::Get_tooltip(float x, float y) const
{
	for(Widgets::const_iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		if((*i)->Covers_point(x, y))
			return (*i)->Get_tooltip(x, y);
	}
	return Widget::Get_tooltip(x, y);
}
