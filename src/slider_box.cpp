#include "slider_box.h"
#include <iostream>
#include <sinxml/sinxml.h>
#include <yaml-cpp/yaml.h>

Slider_box::Slider_box()
:child(NULL)
{
	hslider.Set_event_queue(&slider_events);
	vslider.Set_event_queue(&slider_events);
	hslider.Set_pane_fraction(0);
	vslider.Set_pane_fraction(0);
}

Slider_box::Slider_box(const Slider_box& o)
:Container(o)
,child(NULL)
{
	hslider.Set_event_queue(&slider_events);
	vslider.Set_event_queue(&slider_events);
	hslider.Set_pane_fraction(0);
	vslider.Set_pane_fraction(0);
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
	Handle_child_resize();
}

Widget* Slider_box::Get_child() const
{
	return child;
}

void Slider_box::Handle_event(const ALLEGRO_EVENT& event)
{
	Vector2 s = Get_size();
	if(child)
	{
		child->Handle_event(event);
		Vector2 cs = child->Get_size();
		if(cs.x>s.x)
			hslider.Handle_event(event);
		if(cs.y>s.y)
			vslider.Handle_event(event);
	}

	while(!slider_events.Empty())
	{
		if(child)
		{
			const Event& e = slider_events.Front();
			if(e.type == "moved")
			{
				Vector2 p = Get_position();
				Vector2 downsize(Get_value(SLIDER_WIDTH), Get_value(SLIDER_HEIGHT));
				Vector2 ds = child->Get_size()-s+downsize;
				
				child->Set_position(p-Vector2(ds.x*hslider.Get_pane_fraction(), ds.y*vslider.Get_pane_fraction()));
			}
		}
		slider_events.Pop();
	}
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

Widgets Slider_box::Get_children() const
{
	Widgets w;
	w.push_back(child);
	return w;
}

const Horizontal_slider& Slider_box::Get_hslider() const
{
	return hslider;
}

const Vertical_slider& Slider_box::Get_vslider() const
{
	return vslider;
}

void Slider_box::Handle_child_resize()
{
	Vector2 p = Get_position();
	Vector2 s = Get_size();
	Vector2 downsize(Get_value(SLIDER_WIDTH), Get_value(SLIDER_HEIGHT));
	if(child)
	{
		child->Set_size(child->Request_size());
		Child_resized();
		
		Vector2 cs = child->Get_size();
		if(cs.x<=s.x)
			hslider.Set_pane_fraction(0);
		if(cs.y<=s.y)
			vslider.Set_pane_fraction(0);
	}
}

using namespace sinxml;
sinxml::Element* Slider_box::To_xml() const
{
	Element* e_container = Container::To_xml();
	if(!e_container)
		return NULL;
	Element* e_self = new Element("Slider_box");
	Element* e_base = new Element("base");
	e_self->Add_child(e_base);
	e_base->Add_child(e_container);

	if(child && child->Get_name() != "")
	{
		Element* e_child = new Element("child", child->Get_name().Cstring());
		e_self->Add_child(e_child);
	}
	return e_self;
}

void Slider_box::To_yaml(YAML::Emitter& out) const
{
	Container::To_yaml(out);
/*	out << YAML::Key << "Slider_box";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::EndMap;
*/
	out << YAML::Key << "children";
	out << YAML::Value << YAML::BeginSeq;
	if(child && child->Get_name() != "")
	{
		out << child->Get_name();
	}

	out << YAML::EndSeq;
}
