#include "horizontal_paned.h"
#include <iostream>

Horizontal_paned::Horizontal_paned()
:left(NULL)
,right(NULL)
,pane_position(0)
,pane_reference(0)
,holding_pane(-1)
{
}

Widget* Horizontal_paned::Clone() const
{
	return new Horizontal_paned(*this);
}

void Horizontal_paned::Set_left(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	else if(left)
		left->Set_parent(NULL);
	left = widget;
}

void Horizontal_paned::Set_right(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	else if(right)
		right->Set_parent(NULL);
	right = widget;
}

Widget* Horizontal_paned::Get_left() const
{
	return left;
}

Widget* Horizontal_paned::Get_right() const
{
	return right;
}

void Horizontal_paned::Set_pane_position(float p)
{
	float max_x = Get_size().x-7;
	pane_position = p<0?0:p>max_x?max_x:p;
	pane_reference = pane_position/max_x;
	Organise();
}

void Horizontal_paned::Set_pane_fraction(float p)
{
	float max_x = Get_size().x-7;
	float pp = p*max_x;
	pane_position = pp<0?0:pp>max_x?max_x:pp;
	pane_reference = p;
}

float Horizontal_paned::Get_pane_position() const
{
	return pane_position;
}

void Horizontal_paned::Organise()
{
	Vector2 position = Get_position();
	Vector2 size = Get_size();
	float leftsize = pane_position;
	float rightsize = size.x-7-leftsize;
	if(left)
	{
		left->Set_position(position);
		left->Set_size(Vector2(leftsize, size.y));
	}
	if(right)
	{
		right->Set_position(Vector2(position.x+leftsize+7, position.y));
		right->Set_size(Vector2(rightsize, size.y));
	}
}

void Horizontal_paned::Handle_event(const ALLEGRO_EVENT& event)
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		float pane_x_diff = event.mouse.x-(p.x + pane_position);
		if(event.mouse.y >= p.y && event.mouse.y <= p.y + s.y
		&& pane_x_diff >= 0 && pane_x_diff < 7)
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
		if(holding_pane != -1)
		{
			float new_x = (event.mouse.x - p.x) - holding_pane;
			float max_x = Get_size().x-7;
			Set_pane_fraction(new_x/max_x);
			Organise();
		}
	}
	if(left)
		left->Handle_event(event);
	if(right)
		right->Handle_event(event);
}

void Horizontal_paned::Resized()
{
	Set_pane_fraction(pane_reference);
	Organise();
}
