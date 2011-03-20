#include "menu.h"
#include <sinxml/sinxml.h>
#include <yaml-cpp/yaml.h>

Menu::Menu()
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Menu::Clone() const
{
	return new Menu(*this);
}

void Menu::Add_option(const std::string& t)
{
	options.push_back(t);
}

int Menu::Get_option_count() const
{
	return options.size();
}

std::string Menu::Get_option(int i) const
{
	if(i < options.size())
		return options[i];
	return "";
}

int Menu::Get_selected_option() const
{
	return selection;
}

using namespace sinxml;
sinxml::Element* Menu::To_xml() const
{
	Element* e_widget = Widget::To_xml();
	if(!e_widget)
		return NULL;
	Element* e_self = new Element("Menu");
	Element* e_base = new Element("base");
	e_self->Add_child(e_base);
	e_base->Add_child(e_widget);

	for(Options::const_iterator i = options.begin(); i != options.end(); ++i)
	{
		Element* e_text = new Element("option", *i);
		e_self->Add_child(e_text);
	}

	return e_self;
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
