#include "layout.h"

void Layout::Add(Widget* widget)
{
	if(widget==NULL)
		return;
	widgets.push_back(widget);
	widget->Set_parent(this);
}

const Widgets& Layout::Get_widgets() const
{
	return widgets;
}

void Layout::Resized()
{
	Organise();
}

void Layout::Handle_event(const ALLEGRO_EVENT& event)
{
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		(*i)->Handle_event(event);
	}
}
