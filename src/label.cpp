#include "label.h"
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

void Label::Set_text(const Ustring& t)
{
	text = t;
	Child_resized();
}

Ustring Label::Get_text() const
{
	return text;
}

void Label::To_yaml(YAML::Emitter& out) const
{
	Widget::To_yaml(out);
	out << YAML::Key << "Label";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "text";
		out << YAML::Value << text.Cstring();
	out << YAML::EndMap;
}

void Label::From_yaml(const YAML::Node& in)
{
	Widget::From_yaml(in);
	const YAML::Node& doc = in["Label"];
	doc["text"] >> text;

	std::cout<<"Loaded Label: "<<text<<std::endl;
	Calculate_request_size();
}
