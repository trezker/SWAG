#include "layout.h"
#include "widget.h"
#include <sstream>

Layout::Layout()
:root(NULL)
{
}

Layout::~Layout()
{
	for(Name_to_widget::iterator i = name_to_widget.begin(); i != name_to_widget.end(); ++i)
	{
		delete i->second;
	}
}

void Layout::Set_filename(const std::string& fn)
{
	filename = fn;
}

const std::string& Layout::Get_filename() const
{
	return filename;
}

bool Layout::Load()
{
	
}

bool Layout::Save() const
{
}

Widget* Layout::Get_root() const
{
	return root;
}

std::string Layout::Add_widget(const std::string& name, Widget* widget, Widget* parent)
{
	if(name == "")
		return "";
	if(widget_to_name.find(widget) != widget_to_name.end())
		return "";
	std::string tryname = name;
	int instance = 1;
	while(name_to_widget.find(tryname) != name_to_widget.end())
	{
		++instance;
		std::stringstream ss;
		ss<<instance;
		std::string is;
		ss>>is;
		tryname = name + is;
	}
	if(parent == NULL)
	{
		Clear();
		root = parent;
	}
	name_to_widget[tryname] = widget;
	widget_to_name[widget] = tryname;
	return tryname;
}

void Layout::Delete_widget(const std::string& name)
{
	Delete_widget(name_to_widget[name]);
}

void Layout::Delete_widget(Widget* widget)
{
	if(widget == root)
	{
		Clear();
	}
	else
	{
		delete widget;
		name_to_widget.erase(name_to_widget.find(widget_to_name[widget]));
		widget_to_name.erase(widget_to_name.find(widget));
	}
}

void Layout::Clear()
{
	root = NULL;
	for(Name_to_widget::iterator i = name_to_widget.begin(); i != name_to_widget.end(); ++i)
	{
		delete i->second;
	}
	name_to_widget.clear();
	widget_to_name.clear();
}
