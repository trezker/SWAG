#include "inputbox.h"
#include "event_queue.h"

Inputbox::Inputbox()
:pressed(false)
,mouse_over(false)
,text(al_ustr_new(""))
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
		}
	}
	if(pressed && ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		pressed = false;
		if(mouse_over && !has_focus)
		{
			has_focus = true;
			Push_event(Event(this, "got focus"));
		}
		else if(has_focus)
		{
			has_focus = false;
			Push_event(Event(this, "lost focus"));
		}
	}
	if(has_focus)
	{
/*		if(event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
			{
				int pos = al_ustr_offset(text, -1);
				al_ustr_remove_chr(text, pos);
				return;
			}
		}
*/		if(ALLEGRO_EVENT_KEY_CHAR == event.type)
		{
			if(ALLEGRO_KEY_BACKSPACE == event.keyboard.keycode)
			{
				int pos = al_ustr_offset(text, -1);
				al_ustr_remove_chr(text, pos);
				return;
			}
			else
			{
				al_ustr_append_chr(text, event.keyboard.unichar);
			}
		}
	}
}

bool Inputbox::Is_pressed() const
{
	return pressed;
}
