#include "button.h"
#include "event_queue.h"
#include <yaml-cpp/yaml.h>

Button::Button()
:pressed(false)
,mouse_over(false)
,toggle(false)
,active(false)
{
	Enable_fixed_height();
	Enable_fixed_width();
}

Widget* Button::Clone() const
{
	return new Button(*this);
}

void Button::Set_text(const Ustring& t)
{
	text = t;
	Calculate_request_size();
	Child_resized();
}

const Ustring& Button::Get_text() const
{
	return text;
}

void Button::Handle_event(const ALLEGRO_EVENT& event)
{
	if(ALLEGRO_EVENT_MOUSE_AXES == event.type
	|| ALLEGRO_EVENT_MOUSE_BUTTON_DOWN == event.type
	|| ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		Vector2 p = Get_position();
		Vector2 s = Get_size();
		int emx = event.mouse.x;
		int emy = event.mouse.y;
		if(!mouse_over && Covers_point(emx, emy))//!(emx<p.x || emx>p.x+s.x || emy<p.y || emy>p.y+s.y))
		{
			mouse_over = true;
			Push_event(Event(this, "enter"));
		}
		else if(mouse_over && !Covers_point(emx, emy))//(emx<p.x || emx>p.x+s.x || emy<p.y || emy>p.y+s.y))
		{
			mouse_over = false;
			Push_event(Event(this, "leave"));
		}
	}
	if(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN == event.type)
	{
		if(mouse_over)
		{
			pressed = true;
			Push_event(Event(this, "pressed"));
		}
	}
	if(pressed && ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		pressed = false;
		Push_event(Event(this, "released"));
		if(mouse_over)
		{
			Push_event(Event(this, "clicked"));
			if(toggle)
			{
				if(active)
					Deactivate();
				else
					Activate();
			}
		}
	}
}

bool Button::Is_pressed() const
{
	return pressed;
}

bool Button::Is_mouse_over() const
{
	return mouse_over;
}

void Button::Set_toggle(bool t)
{
	toggle = t;
}

bool Button::Is_toggle() const
{
	return toggle;
}

void Button::Activate()
{
	active = true;
	Push_event(Event(this, "activated"));
}

void Button::Deactivate()
{
	active = false;
	Push_event(Event(this, "deactivated"));
}

void Button::Set_active(bool iactive) {
	active = iactive;
}

bool Button::Is_active() const
{
	return active;
}

void Button::To_yaml(YAML::Emitter& out) const
{
	Widget::To_yaml(out);
	out << YAML::Key << "Button";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "text";
		out << YAML::Value << text.Cstring();
		out << YAML::Key << "toggle";
		out << YAML::Value << (toggle?"true":"false");
	out << YAML::EndMap;
}

void Button::From_yaml(const YAML::Node& in)
{
	Widget::From_yaml(in);
	const YAML::Node& doc = in["Button"];
	doc["text"] >> text;
	Ustring b;
	doc["toggle"] >> b;
	toggle = (b=="true");
	std::cout<<"Loaded button: "<<text<<toggle<<std::endl;
	Calculate_request_size();
}
