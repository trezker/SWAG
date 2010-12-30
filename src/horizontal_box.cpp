#include "horizontal_box.h"
#include "widget_view.h"
#include <iostream>

void Horizontal_box::Organise()
{
	float minimum_width = 0;
	int num_dynamic = 0;
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		minimum_width += (*i)->Request_size().x;
		if(!(*i)->Has_fixed_width())
			++num_dynamic;
	}

	float dynamic_width = (Get_size().x - minimum_width)/num_dynamic;
	if(dynamic_width<0)
		dynamic_width = 0;
	Vector2 position = Get_position();
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		Vector2 size = (*i)->Request_size();
		if(!(*i)->Has_fixed_width())
			size.x += dynamic_width;
		if(!(*i)->Has_fixed_height())
			size.y = Get_size().y;
		(*i)->Set_size(size);
		(*i)->Set_position(position);
		position.x += size.x;
	}
}
