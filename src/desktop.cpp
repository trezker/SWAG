#include "desktop.h"
#include <iostream>
#include <sinxml/sinxml.h>
#include <yaml-cpp/yaml.h>

Desktop::Desktop()
:child(NULL)
,tooltip_countdown(0.5)
{
}

Widget* Desktop::Clone() const
{
	return new Desktop(*this);
}

void Desktop::Set_child(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	if(child)
		child->Set_parent(NULL);
	child = widget;
	child->Set_size(Get_size());
}

Widget* Desktop::Get_child() const
{
	return child;
}

void Desktop::Reset_tooltip()
{
	tooltip_countdown = 0.5;
	Set_tooltip("");
}

void Desktop::Handle_event(const ALLEGRO_EVENT& event)
{
	if (ALLEGRO_EVENT_DISPLAY_RESIZE == event.type)
	{
		Set_size(Vector2(event.display.width, event.display.height));
	}
	if(ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY == event.type)
	{
		Reset_tooltip();
	}
	if (ALLEGRO_EVENT_MOUSE_AXES == event.type)
	{
		if(!(child && tooltip_countdown<=0
		&& Get_tooltip() == child->Get_tooltip(event.mouse.x, event.mouse.y)))
		{
			Reset_tooltip();
			tooltip_position.Set(event.mouse.x, event.mouse.y);
		}
	}

	if(ALLEGRO_EVENT_TIMER == event.type)
	{
		ALLEGRO_MOUSE_STATE mouse_state;
		al_get_mouse_state(&mouse_state);

		if(tooltip_countdown <= 0 && Covers_point(mouse_state.x, mouse_state.y))
		{
			if(child)
				Set_tooltip(child->Get_tooltip(tooltip_position.x, tooltip_position.y));
		}
		else
			tooltip_countdown -= al_get_timer_speed(event.timer.source);
	}
	
	if(child)
	{
		child->Handle_event(event);
	}
}

void Desktop::Resized()
{
	if(child)
	{
		child->Set_size(Get_size());
	}
}

bool Desktop::Add_child(Widget* c)
{
	if(!child)
	{
		Set_child(c);
		return true;
	}
	return false;
}

void Desktop::Remove_child(Widget* c)
{
	child = NULL;
}

Vector2 Desktop::Get_tooltip_position() const
{
	return tooltip_position;
}

using namespace sinxml;
sinxml::Element* Desktop::To_xml() const
{
	Element* e_container = Container::To_xml();
	if(!e_container)
		return NULL;
	Element* e_self = new Element("Desktop");
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

void Desktop::To_yaml(YAML::Emitter& out) const
{
	Container::To_yaml(out);
/*	out << YAML::Key << "Desktop";
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
