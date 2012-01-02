#include "dropdown_menu.h"
#include "event_queue.h"
#include "desktop.h"
#include <yaml-cpp/yaml.h>

Dropdown_menu::Dropdown_menu()
:open(false)
,child(NULL)
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Dropdown_menu::Clone() const
{
	return new Dropdown_menu(*this);
}

bool Dropdown_menu::Is_open() const{
	return open;
}

void Dropdown_menu::Open(){
	if(!child)
		return;
	Desktop* desktop = Get_desktop();
	desktop->Push_top(child);
	open = true;
}

void Dropdown_menu::Close(){
	if(!child)
		return;
	Desktop* desktop = Get_desktop();
	desktop->Pop_top();
	open = false;
}

void Dropdown_menu::Close_child(Widget* c){
	Close();
}

Widget* Dropdown_menu::Get_child() const{
	return child;
}

bool Dropdown_menu::Add_child(Widget* c){
	if(!child)
	{
		child = c;
		child->Set_parent(this);
		Organise();
		return true;
	}
	return false;
}

void Dropdown_menu::Remove_child(Widget* c){
	child = NULL;
}

Widgets Dropdown_menu::Get_children() const{
	Widgets w;
	if(child) {
		w.push_back(child);
	}
	return w;
}

void Dropdown_menu::Organise()
{
	Vector2 p = Get_position();
	p.y += Get_value(SELF_HEIGHT);
	Vector2 s = Get_size();
	s.y -= Get_value(SELF_HEIGHT);

	if(child)
	{
		child->Set_position(p);
	}
}

void Dropdown_menu::Resized()
{
	Organise();
}

void Dropdown_menu::Handle_event(const ALLEGRO_EVENT& event)
{
	if(ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		Vector2 p = Get_position();
		bool covers_point = Covers_point(event.mouse.x, event.mouse.y);

		if(covers_point)
		{
			if(Is_open())
				Close();
			else
				Open();
		}
	}

}

void Dropdown_menu::To_yaml(YAML::Emitter& out) const
{
	Container::To_yaml(out);
	out << YAML::Key << "Dropdown_menu";

	out << YAML::Key << "children";
	out << YAML::Value << YAML::BeginSeq;
	if(child && child->Get_name() != "")
	{
		out << child->Get_name();
	}

	out << YAML::EndSeq;
}

void Dropdown_menu::From_yaml(const YAML::Node& in)
{
	Container::From_yaml(in);
	const YAML::Node& doc = in["Dropdown_menu"];
}
