#include "expander.h"
#include "widget_view.h"
#include "event_queue.h"
#include <algorithm>

Expander::Expander()
:open(false)
,selected(false)
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

void Expander::Add_child(Expander* c)
{
	if(!c)
		return;
	children.push_back(c);
	c->Set_parent(this);
	Organise();
	Child_resized();
}

void Expander::Remove_child(Expander* c)
{
	Expanders::iterator i = std::find(children.begin(), children.end(), c);
	if(i!=children.end())
		children.erase(i);
	Organise();
	Child_resized();
}

const Expanders& Expander::Get_children() const
{
	return children;
}

Expanders& Expander::Get_children()
{
	return children;
}

bool Expander::Is_selected() const
{
	return selected;
}

void Expander::Child_selected()
{
	Expander* parent = dynamic_cast<Expander*>(Get_parent());
	if(parent)
		parent->Child_selected();
	Deselect();
}

void Expander::Deselect()
{
	bool event_deselected = selected;
	selected = false;
	for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Deselect();
	}
	if(event_deselected)
		Push_event(Event(this, "deselected"));
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

	for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
	{
		Vector2 size = (*i)->Request_size();
		(*i)->Set_size(size);
		(*i)->Set_position(Vector2(x, y));
		y += size.y;
	}
}

void Expander::Handle_event(const ALLEGRO_EVENT& event)
{
	if(Is_open())
	{
		for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Handle_event(event);
		}
	}

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
			else
			{
				Child_selected();
				selected = true;
				Push_event(Event(this, "selected"));
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
	Expander* e = dynamic_cast<Expander*>(c);
	if(e)
	{
		Add_child(e);
		return true;
	}
	return false;
}
