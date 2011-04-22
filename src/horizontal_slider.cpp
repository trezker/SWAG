#include "horizontal_slider.h"
#include <iostream>
#include "event_queue.h"

Horizontal_slider::Horizontal_slider()
{
	Enable_fixed_height();
}

Widget* Horizontal_slider::Clone() const
{
	return new Horizontal_slider(*this);
}

void Horizontal_slider::Set_pane_position(float p)
{
	float max_x = Get_size().x-pane_size;
	pane_position = p<0?0:p>max_x?max_x:p;
	pane_reference = pane_position/max_x;
	Push_event(Event(this, "moved"));
}

void Horizontal_slider::Set_pane_fraction(float p)
{
	float max_x = Get_size().x-pane_size;
	float pp = p*max_x;
	pane_position = pp<0?0:pp>max_x?max_x:pp;
	pane_reference = p<0?0:p>1?1:p;
	Push_event(Event(this, "moved"));
}

void Horizontal_slider::Handle_event(const ALLEGRO_EVENT& event)
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		float pane_x_diff = event.mouse.x-(p.x + pane_position);
		if(event.mouse.y >= p.y && event.mouse.y <= p.y + s.y
		&& pane_x_diff >= 0 && pane_x_diff < pane_size)
		{
			holding_pane = pane_x_diff;
		}
	}
	if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		holding_pane = -1;
	}
	if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		if(holding_pane != -1 && event.mouse.dx)
		{
			float new_x = (event.mouse.x - p.x) - holding_pane;
			float max_x = Get_size().x-pane_size;
			Set_pane_fraction(new_x/max_x);
		}
        if(event.mouse.dw)
        {
			float p = Get_pane_fraction();
			p -= event.mouse.dz/scroll_units;
			Set_pane_fraction(p);
        }
	}
}
