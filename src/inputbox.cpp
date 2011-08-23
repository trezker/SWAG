#include "inputbox.h"
#include "event_queue.h"
#include <iostream>
#include <clipboard/clipboard.h>
#include <sinxml/sinxml.h>
#include <yaml-cpp/yaml.h>

Inputbox::Inputbox()
:pressed(false)
,has_focus(false)
,mouse_over(false)
,cursor(0)
,selection_start(0)
,selection_end(0)
{
	Enable_fixed_height();
}

Inputbox::Inputbox(const Inputbox& o)
:Widget(o)
,pressed(false)
,has_focus(false)
,mouse_over(false)
,text(o.text)
,cursor(0)
,selection_start(0)
,selection_end(0)
{
}

Widget* Inputbox::Clone() const
{
	return new Inputbox(*this);
}

void Inputbox::Set_text(const Ustring& t)
{
	text = t;
}

const Ustring& Inputbox::Get_text() const
{
	return text;
}

bool Inputbox::Remove_range()
{
	if(selection_start != selection_end)
	{
		if(selection_start < selection_end)
		{
			text.Remove_range(selection_start, selection_end);
			cursor = selection_start;
		}
		else
		{
			text.Remove_range(selection_end, selection_start);
			cursor = selection_end;
		}
		selection_start = cursor;
		selection_end = cursor;
		return true;
	}
	return false;
}

void Inputbox::Handle_event(const ALLEGRO_EVENT& event)
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
		if(mouse_over && !Covers_point(emx, emy))//(emx<p.x || emx>p.x+s.x || emy<p.y || emy>p.y+s.y))
		{
			mouse_over = false;
			Push_event(Event(this, "leave"));
		}
		if(pressed)
		{
			selection_end = Get_value(POSITION);
			cursor = selection_end;
		}
	}
	if(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN == event.type)
	{
		if(mouse_over)
		{
			pressed = true;
			selection_start = Get_value(POSITION);
			cursor = selection_start;
			selection_end = cursor;
		}
	}
	if(ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		if(pressed && mouse_over)
		{
			pressed = false;
			if(!has_focus)
				Push_event(Event(this, "got focus"));
			has_focus = true;
		}
		else
		{
 			if(has_focus)
				Push_event(Event(this, "lost focus"));
 			has_focus = false;
		}
	}
	if(has_focus)
	{
		if(ALLEGRO_EVENT_KEY_CHAR == event.type)
		{
			Set_value(FLASH, 0);
			if(ALLEGRO_KEY_BACKSPACE == event.keyboard.keycode)
			{
				if(!Remove_range() && cursor>0)
				{
					--cursor;
					text.Remove_chr(cursor);
					selection_start = cursor;
					selection_end = cursor;
					Push_event(Event(this, "changed"));
				}
				return;
			}
			else if(ALLEGRO_KEY_DELETE == event.keyboard.keycode)
			{
				if(!Remove_range() && cursor < text.Length())
				{
					text.Remove_chr(cursor);
					Push_event(Event(this, "changed"));
				}
				return;
			}
			else if(ALLEGRO_KEY_ENTER == event.keyboard.keycode)
			{
				Push_event(Event(this, "activated"));
				return;
			}
			else if(ALLEGRO_KEY_LEFT == event.keyboard.keycode)
			{
				if(cursor>0)
				{
					--cursor;
					if(!(event.keyboard.modifiers&ALLEGRO_KEYMOD_SHIFT))
						selection_start = cursor;
					selection_end = cursor;
				}
			}
			else if(ALLEGRO_KEY_RIGHT == event.keyboard.keycode)
			{
				if(cursor < text.Length())
				{
					++cursor;
					if(!(event.keyboard.modifiers&ALLEGRO_KEYMOD_SHIFT))
						selection_start = cursor;
					selection_end = cursor;
				}
			}
			else if(ALLEGRO_KEY_END == event.keyboard.keycode)
			{
				cursor = text.Length();
				if(!(event.keyboard.modifiers&ALLEGRO_KEYMOD_SHIFT))
					selection_start = cursor;
				selection_end = cursor;
			}
			else if(ALLEGRO_KEY_HOME == event.keyboard.keycode)
			{
				cursor = 0;
				if(!(event.keyboard.modifiers&ALLEGRO_KEYMOD_SHIFT))
					selection_start = cursor;
				selection_end = cursor;
			}
			else if(ALLEGRO_KEY_C == event.keyboard.keycode
			&& (event.keyboard.modifiers&ALLEGRO_KEYMOD_CTRL))
			{
				Ustring sub = text.Substring(selection_start, selection_end);
				const char *cstr = sub.Cstring();
				Set_clipboard_text(cstr, strlen(cstr) + 1);
			}
			else if(ALLEGRO_KEY_V == event.keyboard.keycode
			&& (event.keyboard.modifiers&ALLEGRO_KEYMOD_CTRL))
			{
				std::string paste = Get_clipboard_text();
				text.Insert(cursor, paste.c_str());
				Push_event(Event(this, "changed"));
			}
			else
			{
//				std::cout<<event.keyboard.unichar<<std::endl;
				if(event.keyboard.unichar != -1)
					Remove_range();
				if(text.Insert(cursor, event.keyboard.unichar)>0)
				{
					++cursor;
					selection_start = cursor;
					selection_end = cursor;
					Push_event(Event(this, "changed"));
				}
			}
		}
	}
}

bool Inputbox::Is_pressed() const
{
	return pressed;
}

bool Inputbox::Has_focus() const
{
	return has_focus;
}

int Inputbox::Cursor_position() const
{
	return cursor;
}

int Inputbox::Get_selection_start() const
{
	return selection_start;
}

int Inputbox::Get_selection_end() const
{
	return selection_end;
}

using namespace sinxml;
sinxml::Element* Inputbox::To_xml() const
{
	Element* e_widget = Widget::To_xml();
	if(!e_widget)
		return NULL;
	Element* e_self = new Element("Inputbox");
	Element* e_base = new Element("base");
	e_self->Add_child(e_base);
	e_base->Add_child(e_widget);

	Element* e_text = new Element("text", text.Cstring());
	e_self->Add_child(e_text);

	return e_self;
}

void Inputbox::To_yaml(YAML::Emitter& out) const
{
	Widget::To_yaml(out);
	out << YAML::Key << "Inputbox";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "text";
		out << YAML::Value << text;
	out << YAML::EndMap;
}

void Inputbox::From_yaml(const YAML::Node& in)
{
	Widget::From_yaml(in);
	const YAML::Node& doc = in["Inputbox"];
	doc["text"] >> text;

	std::cout<<"Loaded Inputbox: "<<text<<std::endl;
}
