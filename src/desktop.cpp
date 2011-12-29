#include "desktop.h"
#include <iostream>
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

	if(!top.empty())
	{
		top.back()->Handle_event(event);
	} else { //TODO: Figure out if letting through certain events is neccesary. (mouse_up, mouse_axis)
		if(child)
		{
			child->Handle_event(event);
		}
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

Widgets Desktop::Get_children() const
{
	Widgets w;
	if(child) {
		w.push_back(child);
	}
	return w;
}

Vector2 Desktop::Get_tooltip_position() const
{
	return tooltip_position;
}

void Desktop::Push_top(Widget* widget)
{
	top.push_back(widget);
}

void Desktop::Pop_top()
{
	top.pop_back();
}

const Widgets& Desktop::Get_top() const
{
	return top;
}

void Desktop::To_yaml(YAML::Emitter& out) const
{
	Container::To_yaml(out);

	out << YAML::Key << "children";
	out << YAML::Value << YAML::BeginSeq;
	if(child && child->Get_name() != "")
	{
		out << child->Get_name();
	}

	out << YAML::EndSeq;
}
