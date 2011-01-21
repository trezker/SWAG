#include "vertical_slider.h"
#include "event_queue.h"
#include <iostream>

Vertical_slider::Vertical_slider()
:pane_position(0)
,pane_reference(0.5)
,pane_size(30)
,holding_pane(-1)
{
	Enable_fixed_width();
}

Widget* Vertical_slider::Clone() const
{
	return new Vertical_slider(*this);
}

void Vertical_slider::Set_pane_position(float p)
{
	float max_y = Get_size().y-pane_size;
	pane_position = p<0?0:p>max_y?max_y:p;
	pane_reference = pane_position/max_y;
	Push_event(Event(this, "moved"));
}

void Vertical_slider::Set_pane_fraction(float p)
{
	float max_y = Get_size().y-pane_size;
	float pp = p*max_y;
	pane_position = pp<0?0:pp>max_y?max_y:pp;
	pane_reference = p;
	Push_event(Event(this, "moved"));
}

float Vertical_slider::Get_pane_position() const
{
	return pane_position;
}

float Vertical_slider::Get_pane_fraction() const
{
	return pane_reference;
}

void Vertical_slider::Set_pane_size(int s)
{
	pane_size = s;
}

int Vertical_slider::Get_pane_size() const
{
	return pane_size;
}

void Vertical_slider::Handle_event(const ALLEGRO_EVENT& event)
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		float pane_y_diff = event.mouse.y-(p.y + pane_position);
		if(event.mouse.x >= p.x && event.mouse.x <= p.x + s.x
		&& pane_y_diff >= 0 && pane_y_diff < pane_size)
		{
			holding_pane = pane_y_diff;
		}
	}
	if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		holding_pane = -1;
	}
	if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		if(holding_pane != -1 && event.mouse.dy)
		{
			float new_y = (event.mouse.y - p.y) - holding_pane;
			float max_y = Get_size().y-pane_size;
			float new_f = new_y/max_y;
			Set_pane_fraction(new_y/max_y);
		}
	}
}

void Vertical_slider::Resized()
{
	Set_pane_fraction(pane_reference);
}
