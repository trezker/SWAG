#include "vertical_paned.h"
#include <iostream>

Vertical_paned::Vertical_paned()
:top(NULL)
,bottom(NULL)
,pane_position(0)
,pane_reference(0)
,holding_pane(-1)
{
}

Widget* Vertical_paned::Clone() const
{
	return new Vertical_paned(*this);
}

void Vertical_paned::Set_top(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	else if(top)
		top->Set_parent(NULL);
	top = widget;
	Organise();
}

void Vertical_paned::Set_bottom(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	else if(bottom)
		bottom->Set_parent(NULL);
	bottom = widget;
	Organise();
}

Widget* Vertical_paned::Get_top() const
{
	return top;
}

Widget* Vertical_paned::Get_bottom() const
{
	return bottom;
}

void Vertical_paned::Set_pane_position(float p)
{
	float pane_size = Get_value(PANE);
	float max_y = Get_size().y-pane_size;
	pane_position = p<0?0:p>max_y?max_y:p;
	pane_reference = pane_position/max_y;
	Organise();
}

void Vertical_paned::Set_pane_fraction(float p)
{
	float pane_size = Get_value(PANE);
	float max_y = Get_size().y-pane_size;
	float pp = p*max_y;
	pane_position = pp<0?0:pp>max_y?max_y:pp;
	pane_reference = p;
}

float Vertical_paned::Get_pane_position() const
{
	return pane_position;
}

void Vertical_paned::Organise()
{
	Vector2 position = Get_position();
	Vector2 size = Get_size();
	float topsize = pane_position;
	float pane_size = Get_value(PANE);
	float bottomsize = size.y-pane_size-topsize;
	if(top)
	{
		top->Set_position(position);
		top->Set_size(Vector2(size.x, topsize));
	}
	if(bottom)
	{
		bottom->Set_position(Vector2(position.x, position.y+topsize+pane_size));
		bottom->Set_size(Vector2(size.x, bottomsize));
	}
}

void Vertical_paned::Handle_event(const ALLEGRO_EVENT& event)
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	float pane_size = Get_value(PANE);
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
		if(holding_pane != -1)
		{
			float new_y = (event.mouse.y - p.y) - holding_pane;
			float max_y = Get_size().y-pane_size;
			Set_pane_fraction(new_y/max_y);
			Organise();
		}
	}
	if(top)
		top->Handle_event(event);
	if(bottom)
		bottom->Handle_event(event);
}

void Vertical_paned::Resized()
{
	Set_pane_fraction(pane_reference);
	Organise();
}

bool Vertical_paned::Add_child(Widget* c)
{
	if(!top)
	{
		Set_top(c);
		return true;
	}
	if(!bottom)
	{
		Set_bottom(c);
		return true;
	}
	return false;
}

void Vertical_paned::Remove_child(Widget* c)
{
	if(top == c)
		top = NULL;
	if(bottom == c)
		bottom = NULL;
}

const std::string& Vertical_paned::Get_tooltip(float x, float y) const
{
	if(top && top->Covers_point(x, y))
	{
		return top->Get_tooltip(x, y);
	}
	if(bottom && bottom->Covers_point(x, y))
	{
		return bottom->Get_tooltip(x, y);
	}
}
