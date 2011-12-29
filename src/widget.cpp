#include "widget.h"
#include "widget_view.h"
#include "event_queue.h"
#include "container.h"
#include <iostream>
#include "layout.h"
#include <yaml-cpp/yaml.h>

#include "desktop.h"

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

void Widget::Set_tooltip(const Ustring& t)
{
	tooltip = t;
}

void Widget::Set_name(const Ustring& n)
{
	name = n;
}

const Ustring& Widget::Get_name() const
{
	return name;
}

void Widget::Set_prototype_name(const Ustring& n)
{
	prototype_name = n;
}

const Ustring& Widget::Get_prototype_name() const
{
	return prototype_name;
}

const Ustring& Widget::Get_tooltip() const
{
	return tooltip;
}

const Ustring& Widget::Get_tooltip(float x, float y) const
{
	return tooltip;
}

void Widget::Set_fixed_height(bool active)
{
	fixed_height = active;
	Child_resized();
}

void Widget::Set_fixed_width(bool active)
{
	fixed_width = active;
	Child_resized();
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

Desktop* Widget::Get_desktop()
{
	Desktop* d = dynamic_cast<Desktop*>(this);
	if(d) {
		return d;
	} else {
		return parent->Get_desktop();
	}
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

void Widget::To_yaml(YAML::Emitter& out) const
{
	out << YAML::Key << "Widget";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "tooltip";
		out << YAML::Value << tooltip.Cstring();
		out << YAML::Key << "fixed_width";
		out << YAML::Value << (fixed_width?"true":"false");
		out << YAML::Key << "fixed_height";
		out << YAML::Value << (fixed_height?"true":"false");
	out << YAML::EndMap;
}

void Widget::From_yaml(const YAML::Node& in)
{
	const YAML::Node& doc = in["Widget"];
	doc["tooltip"] >> tooltip;
	Ustring b;
	doc["fixed_width"] >> b;
	fixed_width = (b=="true");
	doc["fixed_height"] >> b;
	fixed_height = (b=="true");
	std::cout<<"Loaded widget: "<<tooltip<<fixed_width<<fixed_height<<std::endl;
}
