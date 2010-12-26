#include "layout.h"

void Layout::Add(Widget* widget)
{
	widgets.push_back(widget);
}

const Widgets& Layout::Get_widgets() const
{
	return widgets;
}

void Layout::Resized()
{
	Organise();
}

void Layout::Event(const ALLEGRO_EVENT& event)
{
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		(*i)->Event(event);
	}
}
