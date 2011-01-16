#include "context_menu.h"

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

void Context_menu::Set_text(const std::string& t)
{
	text = t;
}

std::string Context_menu::Get_text() const
{
	return text;
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
