#include "slider_box.h"
#include <iostream>

Slider_box::Slider_box()
:child(NULL)
{
}

Widget* Slider_box::Clone() const
{
	return new Slider_box(*this);
}

void Slider_box::Set_child(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	if(child)
		child->Set_parent(NULL);
	child = widget;
	Vector2 downsize(Get_value(SLIDER_WIDTH), Get_value(SLIDER_HEIGHT));
	child->Set_size(Get_size()-downsize);
}

Widget* Slider_box::Get_child() const
{
	return child;
}

void Slider_box::Handle_event(const ALLEGRO_EVENT& event)
{
	if(child)
		child->Handle_event(event);
	hslider.Handle_event(event);
	vslider.Handle_event(event);
}

void Slider_box::Resized()
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	Vector2 downsize(Get_value(SLIDER_WIDTH), Get_value(SLIDER_HEIGHT));
	if(child)
		child->Set_size(child->Request_size());
	hslider.Set_position(Vector2(p.x, p.y+s.y-downsize.y));
	hslider.Set_size(Vector2(s.x-downsize.x, downsize.y));
	vslider.Set_position(Vector2(p.x+s.x-downsize.x, p.y));
	vslider.Set_size(Vector2(downsize.x, s.y-downsize.y));
}

bool Slider_box::Add_child(Widget* c)
{
	if(!child)
	{
		Set_child(c);
		return true;
	}
	return false;
}

void Slider_box::Remove_child(Widget* c)
{
	child = NULL;
}

const Horizontal_slider& Slider_box::Get_hslider() const
{
	return hslider;
}

const Vertical_slider& Slider_box::Get_vslider() const
{
	return vslider;
}
