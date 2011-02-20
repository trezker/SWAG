#include "widget.h"
#include "widget_view.h"
#include "event_queue.h"
#include "container.h"
#include <iostream>
#include <sinxml/sinxml.h>
#include "layout.h"

Widget::Widget()
:view(NULL)
,fixed_width(false)
,fixed_height(false)
,parent(NULL)
,event_queue(NULL)
{
}

Widget::~Widget()
{
}

Widget* Widget::Clone() const
{
	return new Widget(*this);
}

void Widget::Handle_event(const ALLEGRO_EVENT& event)
{
}

void Widget::Set_view(Widget_view* v)
{
	view = v;
}
/*
Widget_view* Widget::Get_view() const
{
	return view;
}
*/

float Widget::Get_value(int id) const
{
	return view?view->Get_value(id, *this):0;
}

void Widget::Set_value(int id, float v)
{
	if(view)
		view->Set_value(id, v);
}

Vector2 Widget::Request_size() const
{
	if(view)
		return view->Request_size(*this);
	return Vector2();
}

void Widget::Render() const
{
	if(view)
		view->Render(*this);
}

void Widget::Set_position(Vector2 p)
{
	position = p;
	Resized();
}

void Widget::Set_size(Vector2 s)
{
	size = s;
	Resized();
}

Vector2 Widget::Get_position() const
{
	return position;
}

Vector2 Widget::Get_size() const
{
	return size;
}

void Widget::Set_tooltip(const std::string& t)
{
	tooltip = t;
}

void Widget::Set_name(const std::string& n)
{
	name = n;
}

const std::string& Widget::Get_name() const
{
	return name;
}

void Widget::Set_prototype_name(const std::string& n)
{
	prototype_name = n;
}

const std::string& Widget::Get_prototype_name() const
{
	return prototype_name;
}

const std::string& Widget::Get_tooltip() const
{
	return tooltip;
}

const std::string& Widget::Get_tooltip(float x, float y) const
{
	return tooltip;
}

void Widget::Enable_fixed_height()
{
	fixed_height = true;
	Child_resized();
}

void Widget::Enable_fixed_width()
{
	fixed_width = true;
	Child_resized();
}

void Widget::Disable_fixed_height()
{
	fixed_height = false;
	Child_resized();
}

void Widget::Disable_fixed_width()
{
	fixed_width = false;
	Child_resized();
}

bool Widget::Has_fixed_height() const
{
	return fixed_height;
}

bool Widget::Has_fixed_width() const
{
	return fixed_width;
}

void Widget::Resized()
{
}

void Widget::Push_event(const Event& event)
{
	if(event_queue)
	{
		event_queue->Push(event);
	}
	if(parent)
	{
		parent->Push_event(event);
	}
}

void Widget::Set_event_queue(Event_queue* e)
{
	event_queue = e;
}

Event_queue* Widget::Get_event_queue() const
{
	return event_queue;
}

void Widget::Set_parent(Container* p)
{
	parent = p;
}

Container* Widget::Get_parent() const
{
	return parent;
}

bool Widget::Covers_point(float x, float y)
{
	Vector2 s = Get_size();
	Vector2 p = Get_position();
	return (x > p.x && y > p.y+1
		&& x < p.x+s.x-1 && y < p.y + s.y);
}

void Widget::Child_resized()
{
	if(parent)
		parent->Handle_child_resize();
}

using namespace sinxml;
sinxml::Element* Widget::To_xml() const
{
/*	if(name == "")
	{
		return NULL;
	}
*/	Element* e_self = new Element("widget");
//	Element* e_name = new Element("name", name);
	Element* e_tooltip = new Element("tooltip", tooltip);
	Element* e_fixed_width = new Element("fixed_width", fixed_width?"true":"false");
	Element* e_fixed_height = new Element("fixed_height", fixed_height?"true":"false");
//	e_self->Add_child(e_name);
	e_self->Add_child(e_tooltip);
	e_self->Add_child(e_fixed_width);
	e_self->Add_child(e_fixed_height);
	return e_self;
}
