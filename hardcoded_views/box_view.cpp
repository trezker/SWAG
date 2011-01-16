#include "box_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <vertical_box.h>
#include <horizontal_box.h>

Vector2 Box_view::Request_size(const Widget& widget) const
{
	Vector2 size;
	const Box& box = dynamic_cast<const Box&>(widget);
	Widgets widgets = box.Get_widgets();
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		Vector2 ws = (*i)->Request_size();
		size.y += ws.y;
		if(ws.x>size.x)
			size.x = ws.x;
	}
	return size;
}

void Box_view::Render(const Widget& widget) const
{
	const Box& box = dynamic_cast<const Box&>(widget);
	Widgets widgets = box.Get_widgets();
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		(*i)->Render();
	}
}

Vector2 HBox_view::Request_size(const Widget& widget) const
{
	Vector2 size;
	const Box& box = dynamic_cast<const Box&>(widget);
	Widgets widgets = box.Get_widgets();
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		Vector2 ws = (*i)->Request_size();
		size.x += ws.x;
		if(ws.y>size.y)
			size.y = ws.y;
	}
	return size;
}

void HBox_view::Render(const Widget& widget) const
{
	const Box& box = dynamic_cast<const Box&>(widget);
	Widgets widgets = box.Get_widgets();
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		(*i)->Render();
	}
}
