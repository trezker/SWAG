#include "horizontal_paned.h"
#include <iostream>
#include <sinxml/sinxml.h>

Horizontal_paned::Horizontal_paned()
:left(NULL)
,right(NULL)
,pane_position(0)
,pane_reference(0.5)
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
	Organise();
}

void Horizontal_paned::Set_right(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	else if(right)
		right->Set_parent(NULL);
	right = widget;
	Organise();
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
	float pane_size = Get_value(PANE);
	float max_x = Get_size().x-pane_size;
	pane_position = p<0?0:p>max_x?max_x:p;
	pane_reference = pane_position/max_x;
	Organise();
}

void Horizontal_paned::Set_pane_fraction(float p)
{
	float pane_size = Get_value(PANE);
	float max_x = Get_size().x-pane_size;
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
	float pane_size = Get_value(PANE);
	float leftsize = pane_position;
	float rightsize = size.x-pane_size-leftsize;
	if(left)
	{
		left->Set_position(position);
		left->Set_size(Vector2(leftsize, size.y));
	}
	if(right)
	{
		right->Set_position(Vector2(position.x+leftsize+pane_size, position.y));
		right->Set_size(Vector2(rightsize, size.y));
	}
}

void Horizontal_paned::Handle_event(const ALLEGRO_EVENT& event)
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	float pane_size = Get_value(PANE);
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
		if(holding_pane != -1)
		{
			float new_x = (event.mouse.x - p.x) - holding_pane;
			float max_x = Get_size().x-pane_size;
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

bool Horizontal_paned::Add_child(Widget* c)
{
	if(!left)
	{
		Set_left(c);
		return true;
	}
	if(!right)
	{
		Set_right(c);
		return true;
	}
	return false;
}

void Horizontal_paned::Remove_child(Widget* c)
{
	if(left == c)
		left = NULL;
	if(right == c)
		right = NULL;
}

const std::string& Horizontal_paned::Get_tooltip(float x, float y) const
{
	if(left && left->Covers_point(x, y))
	{
		return left->Get_tooltip(x, y);
	}
	if(right && right->Covers_point(x, y))
	{
		return right->Get_tooltip(x, y);
	}
	return Widget::Get_tooltip(x, y);
}

using namespace sinxml;
sinxml::Element* Horizontal_paned::To_xml() const
{
	Element* e_container = Container::To_xml();
	if(!e_container)
		return NULL;
	Element* e_self = new Element("Horizontal_paned");
	Element* e_base = new Element("base");
	e_self->Add_child(e_base);
	e_base->Add_child(e_container);

	Element* e_left = new Element("child", left?left->Get_name():"");
	e_self->Add_child(e_left);
	Element* e_right = new Element("child", right?right->Get_name():"");
	e_self->Add_child(e_right);
	return e_self;
}

void Horizontal_paned::To_yaml(YAML::Emitter& out) const
{
	out << YAML::Key << "Horizontal_paned";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "children";
		out << YAML::Value << YAML::BeginSeq;
			if(left) out << left->Get_name();
			if(right) out << right->Get_name();
		out << YAML::EndSeq;
	out << YAML::EndMap;
}
