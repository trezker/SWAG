#include "layout.h"
#include "widget.h"
#include <sstream>
#include <sinxml/sinxml.h>
#include <iostream>

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
//TODO: Figure out how to save skin info and prototype name.
/*Check for errors:
 * Child with the same name occuring more than once
 * Reference to child that does not exist
 * Orphan widgets
*/
using namespace sinxml;
bool Layout::Save() const
{
	if(root==NULL)
	{
		std::cout<<"No root widget"<<std::endl;
		return false;
	}
	if(filename=="")
	{
		std::cout<<"No filename"<<std::endl;
		return false;
	}
	Element* e_layout = new Element("layout");
	sinxml::Element* e_root = new Element("root", root->Get_name());
	e_layout->Add_child(e_root);

	Element* e_widgets = new Element("widgets");
	e_layout->Add_child(e_widgets);
	for(Name_to_widget::const_iterator i = name_to_widget.begin(); i != name_to_widget.end(); ++i)
	{
		Element* e_widget = i->second->To_xml();
		if(e_widget)
			e_widgets->Add_child(e_widget);
	}

	sinxml::Document document("1.0");
	document.Set_root(e_layout);
	document.Save_file(filename);
	return true;
}

Widget* Layout::Get_root() const
{
	return root;
}

std::string Layout::Add_widget(const std::string& name, Widget* widget, Widget* parent)
{
	if(name == "")
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
		root = widget;
	}
	name_to_widget[tryname] = widget;
	widget->Set_name(tryname);
	return tryname;
}

void Layout::Delete_widget(const std::string& name)
{
	Delete_widget(name_to_widget[name]);
}

void Layout::Remove_widget(Widget* widget)
{
	name_to_widget.erase(name_to_widget.find(widget->Get_name()));
}

void Layout::Delete_widget(Widget* widget)
{
	if(widget == root)
	{
		Clear();
	}
	else
	{
		name_to_widget.erase(name_to_widget.find(widget->Get_name()));
		delete widget;
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
}
