#include "expander.h"
#include "widget_view.h"
#include "event_queue.h"

Expander::Expander()
:open(false)
,child(NULL)
{
}

Widget* Expander::Clone() const
{
	return new Expander(*this);
}

bool Expander::Is_open() const
{
	return open;
}

void Expander::Open()
{
	open = true;
}

void Expander::Close()
{
	open = false;
}

void Expander::Set_text(const std::string& t)
{
	text = t;
}

const std::string& Expander::Get_text() const
{
	return text;
}

Widget* Expander::Get_child() const
{
	return child;
}

void Expander::Organise()
{
	bool is_open = Is_open();
	Close();
	Vector2 selfsize = Request_size();
	if(is_open)
		Open();

	float indent = Get_value(INDENT);
	float y = Get_position().y;
	float x = Get_position().x + indent;
	y += selfsize.y;

	if(child)
	{
		Vector2 size = child->Request_size();
		child->Set_size(size);
		child->Set_position(Vector2(x, y));
	}
}

void Expander::Handle_event(const ALLEGRO_EVENT& event)
{
	if(child && Is_open())
		child->Handle_event(event);

	if(ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		bool is_open = Is_open();
		Vector2 ps = Get_size();
		if(is_open)
		{
			Close();
			Set_size(Request_size());
		}
		Vector2 p = Get_position();
		Vector2 s = Get_size();
		bool covers_point = Covers_point(event.mouse.x, event.mouse.y);
		if(is_open)
		{
			Open();
			Set_size(ps);
		}
		
		if(covers_point)
		{
			if(event.mouse.x < p.x+Get_value(INDENT))
			{
				if(is_open)
					Close();
				else
					Open();
				Child_resized();
			}
		}
	}
}

void Expander::Resized()
{
	Organise();
}

void Expander::Handle_child_resize()
{
	Resized();
	Child_resized();
}

bool Expander::Add_child(Widget* c)
{
	if(!child)
	{
		child = c;
		return true;
	}
	return false;
}

void Expander::Remove_child(Widget* c)
{
	child = NULL;
	Organise();
}

const std::string& Expander::Get_tooltip(float x, float y) const
{
	if(child && child->Covers_point(x, y))
		return child->Get_tooltip(x, y);
	return Widget::Get_tooltip(x, y);
}
