#include "expander.h"
#include "widget_view.h"
#include "event_queue.h"

Expander::Expander()
:open(false)
,selected(false)
{
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
	Child_resized();
}

const Expanders& Expander::Get_children() const
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

	float y = Get_position().y;
	float x = Get_position().x + selfsize.y/4 + 6;
	y += selfsize.y;

	for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
	{
		Vector2 size = (*i)->Request_size();
		if(!(*i)->Has_fixed_width())
			size.x = Get_size().x;
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
		Close();
		Vector2 s = Request_size();
		if(is_open)
			Open();
		Vector2 p = Get_position();
		
		if(event.mouse.x > p.x && event.mouse.y > p.y
		&& event.mouse.x < p.x+s.x && event.mouse.y < p.y + s.y)
		{
			if(event.mouse.x < p.x+s.y/2)
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
