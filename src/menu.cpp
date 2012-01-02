#include "menu.h"
#include "event_queue.h"
#include "container.h"
#include <yaml-cpp/yaml.h>

Menu::Menu()
:mouse_over(false)
,selection(-1)
,hover(-1)
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Menu::Clone() const
{
	return new Menu(*this);
}

void Menu::Add_option(const Ustring& t)
{
	options.push_back(t);
	Set_size(Request_size());
}

void Menu::Remove_option(int i){
	if(i>=0 && i<options.size()) {
		options.erase(options.begin()+i);
		Set_size(Request_size());
	}
}

int Menu::Get_option_count() const
{
	return options.size();
}

Ustring Menu::Get_option(int i) const
{
	if(i < options.size())
		return options[i];
	return "";
}

int Menu::Get_selected_option() const
{
	return selection;
}

int Menu::Get_hover_option() const
{
	return hover;
}

void Menu::Handle_event(const ALLEGRO_EVENT& event)
{
	if(ALLEGRO_EVENT_MOUSE_AXES == event.type)
	{
		//Todo: implement hover over options.
		Vector2 p = Get_position();
		Vector2 s = Get_size();
		int emx = event.mouse.x;
		int emy = event.mouse.y;
		if(!mouse_over && Covers_point(emx, emy))
		{
			mouse_over = true;
			Push_event(Event(this, "enter"));
		}
		else if(mouse_over && !Covers_point(emx, emy))//(emx<p.x || emx>p.x+s.x || emy<p.y || emy>p.y+s.y))
		{
			mouse_over = false;
			Push_event(Event(this, "leave"));
		}
		if(mouse_over)
		{
			hover = (event.mouse.y - Get_position().y - Get_value(PADDING_TOP)) / Get_value(OPTION_HEIGHT);
			if(hover<0 || hover>=options.size())
				hover = -1;
		}
	}
	if(ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		if(mouse_over)
		{
			selection = (event.mouse.y - Get_position().y - Get_value(PADDING_TOP)) / Get_value(OPTION_HEIGHT);
			Push_event(Event(this, "clicked"));
		}
		Get_parent()->Close_child(this);
	}
}

void Menu::To_yaml(YAML::Emitter& out) const
{
	Widget::To_yaml(out);
/*	out << YAML::Key << "Slider_box";
	out << YAML::Value << YAML::BeginMap;
	out << YAML::EndMap;
*/
	out << YAML::Key << "options";
	out << YAML::Value << YAML::BeginSeq;
	for(Options::const_iterator i = options.begin(); i != options.end(); ++i)
	{
		out << *i;
	}

	out << YAML::EndSeq;
}
