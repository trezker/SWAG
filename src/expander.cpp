#include "expander.h"
#include "widget_view.h"

Expander::Expander()
:open(false)
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

	for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
	{
		Vector2 size = (*i)->Request_size();
		if(!(*i)->Has_fixed_width())
			size.x = Get_size().x;
		(*i)->Set_size(size);
		(*i)->Set_position(Vector2(x, y));
		y += size.y;
	}
}

void Expander::Event(const ALLEGRO_EVENT& event)
{
	if(Is_open())
	{
		for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
		{
			(*i)->Event(event);
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
		&& event.mouse.x < p.x+s.y/2 && event.mouse.y < p.y + s.y)
		{
			if(is_open)
				Close();
			else
				Open();
		}
	}
}
