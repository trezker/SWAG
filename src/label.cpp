#include "label.h"
#include <sinxml/sinxml.h>
#include <yaml-cpp/yaml.h>

Label::Label()
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Label::Clone() const
{
	return new Label(*this);
}

void Label::Set_text(const std::string& t)
{
	text = t;
	Child_resized();
}

std::string Label::Get_text() const
{
	return text;
}

using namespace sinxml;
sinxml::Element* Label::To_xml() const
{
	Element* e_widget = Widget::To_xml();
	if(!e_widget)
		return NULL;
	Element* e_self = new Element("Label");
	Element* e_base = new Element("base");
	e_self->Add_child(e_base);
	e_base->Add_child(e_widget);

	Element* e_text = new Element("text", text);
	e_self->Add_child(e_text);

	return e_self;
}

void Label::To_yaml(YAML::Emitter& out) const
{
	Widget::To_yaml(out);
	out << YAML::Key << "Label";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "text";
		out << YAML::Value << text;
	out << YAML::EndMap;
}
