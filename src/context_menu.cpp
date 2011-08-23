#include "context_menu.h"
#include <sinxml/sinxml.h>
#include <yaml-cpp/yaml.h>

Context_menu::Context_menu()
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Context_menu::Clone() const
{
	return new Context_menu(*this);
}

void Context_menu::Set_context(Widget* widget)
{
	if(widget)
		widget->Set_parent(this);
	if(context)
		context->Set_parent(NULL);
	context = widget;
	context->Set_size(Get_size());
}

Widget* Context_menu::Get_context() const
{
	return context;
}

bool Context_menu::Add_child(Widget* c)
{
	if(!context)
	{
		Set_context(c);
		return true;
	}
	return false;
}

void Context_menu::Remove_child(Widget* c)
{
	context = NULL;
}

Widgets Context_menu::Get_children() const
{
	Widgets w;
	w.push_back(context);
	return w;
}

using namespace sinxml;
sinxml::Element* Context_menu::To_xml() const
{
	Element* e_container = Container::To_xml();
	if(!e_container)
		return NULL;
	Element* e_self = new Element("Context_menu");
	Element* e_base = new Element("base");
	e_self->Add_child(e_base);
	e_base->Add_child(e_container);

	if(context && context->Get_name() != "")
	{
		Element* e_child = new Element("child", context->Get_name().Cstring());
		e_self->Add_child(e_child);
	}
	return e_self;
}

void Context_menu::To_yaml(YAML::Emitter& out) const
{
	Container::To_yaml(out);
	out << YAML::Key << "children";
	out << YAML::Value << YAML::BeginSeq;
	if(context && context->Get_name() != "")
	{
		out << context->Get_name();
	}

	out << YAML::EndSeq;
}
