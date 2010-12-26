#include "vertical_box.h"
#include "widget_view.h"

void Vertical_box::Organise()
{
	float minimum_height = 0;
	int num_dynamic = 0;
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		minimum_height += (*i)->Request_size().y;
		if(!(*i)->Has_fixed_height())
			++num_dynamic;
	}

	float dynamic_height = (Get_size().y - minimum_height)/num_dynamic;
	if(dynamic_height<0)
		dynamic_height = 0;
	Vector2 position = Get_position();
	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		Vector2 size = (*i)->Request_size();
		if(!(*i)->Has_fixed_height())
			size.y += dynamic_height;
		if(!(*i)->Has_fixed_width())
			size.x = Get_size().x;
		(*i)->Set_size(size);
		(*i)->Set_position(position);
		position.y += size.y;
	}
}
