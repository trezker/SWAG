#include "inputbox.h"
#include "event_queue.h"
#include <iostream>

Inputbox::Inputbox()
:pressed(false)
,mouse_over(false)
,text(al_ustr_new(""))
,cursor(0)
{
}

Inputbox::~Inputbox()
{
	al_ustr_free(text);
}

Inputbox::Inputbox(const Inputbox& o)
:Widget(o)
,pressed(false)
,mouse_over(false)
,text(al_ustr_dup(o.text))
,cursor(0)
{
}

Widget* Inputbox::Clone() const
{
	return new Inputbox(*this);
}

void Inputbox::Set_text(const std::string& t)
{
	al_ustr_assign_cstr(text, t.c_str());
}

std::string Inputbox::Get_text() const
{
	return al_cstr(text);
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
	}
	if(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN == event.type)
	{
		if(mouse_over)
		{
			pressed = true;
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
			cursor = Get_value(POSITION);
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
		Set_value(FLASH, 0);
		if(ALLEGRO_EVENT_KEY_CHAR == event.type)
		{
			if(ALLEGRO_KEY_BACKSPACE == event.keyboard.keycode)
			{
				if(cursor>0)
				{
					int pos = al_ustr_offset(text, cursor-1);
					al_ustr_remove_chr(text, pos);
					--cursor;
					Push_event(Event(this, "changed"));
				}
				return;
			}
			else if(ALLEGRO_KEY_DELETE == event.keyboard.keycode)
			{
				if(cursor<al_ustr_length(text))
				{
					int pos = al_ustr_offset(text, cursor);
					al_ustr_remove_chr(text, pos);
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
					--cursor;
			}
			else if(ALLEGRO_KEY_RIGHT == event.keyboard.keycode)
			{
				if(cursor<al_ustr_length(text))
					++cursor;
			}
			else if(ALLEGRO_KEY_END == event.keyboard.keycode)
			{
				cursor = al_ustr_length(text);
			}
			else if(ALLEGRO_KEY_HOME == event.keyboard.keycode)
			{
				cursor = 0;
			}
			else
			{
				int pos = al_ustr_offset(text, cursor);
				if(al_ustr_insert_chr(text, pos, event.keyboard.unichar)>0)
				{
					++cursor;
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
