#include "button.h"
#include "event_queue.h"

Button::Button()
:pressed(false)
,mouse_over(false)
{
	Enable_fixed_height();
	Enable_fixed_width();
}

Widget* Button::Clone() const
{
	return new Button(*this);
}

void Button::Set_text(const std::string& t)
{
	text = t;
}

const std::string& Button::Get_text() const
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
