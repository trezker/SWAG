#include "layout.h"
#include "widget.h"
#include "skin.h"
#include <sstream>
#include <sinxml/sinxml.h>
#include <iostream>
#include "container.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

Layout::Layout()
:root(NULL)
,skin(NULL)
{
}

Layout::~Layout()
{
	for(Name_to_widget::iterator i = name_to_widget.begin(); i != name_to_widget.end(); ++i)
	{
		delete i->second;
	}
}

void Layout::Set_skin(Skin* s)
{
	skin = s;
}

void Layout::Set_filename(const Ustring& fn)
{
	filename = fn;
}

const Ustring& Layout::Get_filename() const
{
	return filename;
}

bool Layout::Load_yaml()
{
	Clear();
	if(filename=="")
	{
		std::cout<<"No filename"<<std::endl;
		return false;
	}
	if(!skin)
	{
		std::cout<<"No skin"<<std::endl;
		return false;
	}
	std::ifstream fin(filename.Cstring());
	YAML::Parser parser(fin);

	YAML::Node doc;
	parser.GetNextDocument(doc);

	typedef std::map<Container*, const YAML::Node*> Widget_children;
	Widget_children widget_children;
	for(unsigned i=0; i<doc["widgets"].size(); i++)
	{
		const YAML::Node &n = doc["widgets"][i];

		Ustring name;
		n["name"] >> name;
		std::string prototype_name;
		n["prototype_name"] >> prototype_name;

		std::cout<<"DEBUG: "<<name<<"/"<<prototype_name<<std::endl;
		//Skip if name is taken
		if(name_to_widget.find(name) != name_to_widget.end())
		{
			std::cout<<"ERROR: Name clash, skipping"<<std::endl;
			continue;
		}
		Widget* w = skin->Clone<Widget>(prototype_name);
		if(!w)
		{
			std::cout<<"ERROR: Prototype missing, skipping"<<std::endl;
			continue;
		}
		//Todo: Call widgets loading function
		name_to_widget[name] = w;
		w->Set_name(name);
		Container* container = dynamic_cast<Container*>(w);
		if(container)
			widget_children[container] = &(n["children"]);
	}
	//Set root widget
	Ustring rootname;
	doc["root"]>>rootname;
	std::cout<<"DEBUG: rootname = "<<rootname<<std::endl;
	//Check validity of root name
	if(name_to_widget.find(rootname) != name_to_widget.end())
		root = name_to_widget[rootname];
	else
		std::cout<<"ERROR: Root name invalid"<<std::endl;

	//Adding widget children
	for(Widget_children::iterator i = widget_children.begin(); i != widget_children.end(); ++i)
	{
		Container* parent = i->first;
		for(unsigned j=0; j<i->second->size(); j++)
		{
			const YAML::Node &n = *i->second;
			Ustring name;
			n[j]>>name;
			std::cout<<"DEBUG: Adding child "<<i->first<<" : "<<name<<std::endl;
			if(name_to_widget.find(name) != name_to_widget.end())
			{
				if(!parent->Add_child(name_to_widget[name]))
					std::cout<<"ERROR: Failed to add"<<std::endl;
			}
			else
				std::cout<<"ERROR: Widget missing"<<std::endl;
		}
	}
	return true;
}

bool Layout::Load()
{
	Clear();
	if(filename=="")
	{
		std::cout<<"No filename"<<std::endl;
		return false;
	}
	if(!skin)
	{
		std::cout<<"No skin"<<std::endl;
		return false;
	}

	sinxml::Document document("1.0");
	if(!document.Load_file(filename.Cstring()))
	{
		std::cout<<"Failed to load xml file"<<std::endl;
		return false;
	}
	typedef std::map<Container*, sinxml::Children> Widget_children;
	Widget_children widget_children;
	sinxml::Element* root_e = document.Get_root();
	sinxml::Children widget_es = root_e->Get_child("widgets")->Get_children();
	for(sinxml::Children::iterator i = widget_es.begin(); i != widget_es.end(); ++i)
	{
		const Ustring& name = (*i)->Get_attribute("name").c_str();
		const std::string& prototype_name = (*i)->Get_attribute("prototype_name");
		std::cout<<"DEBUG: "<<(*i)->Get_name()<<"/"<<name<<"/"<<prototype_name<<std::endl;
		//Skip if name is taken
		if(name_to_widget.find(name) != name_to_widget.end())
		{
			std::cout<<"ERROR: Name clash, skipping"<<std::endl;
			continue;
		}
		Widget* w = skin->Clone<Widget>(prototype_name);
		if(!w)
		{
			std::cout<<"ERROR: Prototype missing, skipping"<<std::endl;
			continue;
		}
		//Todo: Call widgets loading function
		name_to_widget[name] = w;
		w->Set_name(name);
		Container* container = dynamic_cast<Container*>(w);
		if(container)
			widget_children[container] = (*i)->Get_children();
	}
	
	//Set root widget
	sinxml::Element* rootinfo_e = root_e->Get_child("root");
	const Ustring& rootname = rootinfo_e->Get_value().c_str();
	std::cout<<"DEBUG: rootname = "<<rootname<<std::endl;
	//Check validity of root name
	if(name_to_widget.find(rootname) != name_to_widget.end())
		root = name_to_widget[rootname];
	else
		std::cout<<"ERROR: Root name invalid"<<std::endl;

	//Adding widget children
	for(Widget_children::iterator i = widget_children.begin(); i != widget_children.end(); ++i)
	{
		Container* parent = i->first;
		for(sinxml::Children::iterator e = i->second.begin(); e != i->second.end(); ++e)
		{
			if((*e)->Get_name() == "child")
			{
				const Ustring &name = (*e)->Get_value().c_str();
				std::cout<<"DEBUG: Adding child "<<i->first<<" : "<<name<<std::endl;
				if(name_to_widget.find(name) != name_to_widget.end())
				{
					if(!parent->Add_child(name_to_widget[name]))
						std::cout<<"ERROR: Failed to add"<<std::endl;
				}
				else
					std::cout<<"ERROR: Widget missing"<<std::endl;
			}
		}
	}
}
//TODO: Figure out how to save skin info.
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
	sinxml::Element* e_root = new Element("root", root->Get_name().Cstring());
	e_layout->Add_child(e_root);

	Element* e_widgets = new Element("widgets");
	e_layout->Add_child(e_widgets);
	for(Name_to_widget::const_iterator i = name_to_widget.begin(); i != name_to_widget.end(); ++i)
	{
		Element* e_widget = i->second->To_xml();
		if(e_widget)
		{
			e_widgets->Add_child(e_widget);
			e_widget->Set_attribute("name", i->second->Get_name().Cstring());
			e_widget->Set_attribute("prototype_name", i->second->Get_prototype_name());

/*			Element* e_name = new Element("name", i->second->Get_name());
			Element* e_prototype_name = new Element("prototype_name", i->second->Get_prototype_name());
			e_widget->Add_child(e_name);
			e_widget->Add_child(e_prototype_name);
*/		}
	}

	sinxml::Document document("1.0");
	document.Set_root(e_layout);
	document.Save_file(filename.Cstring());
	return true;
}

bool Layout::Save_yaml() const
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
	
	YAML::Emitter out;
	
	out << YAML::BeginMap;
		out << YAML::Key << "root";
		out << YAML::Value << root->Get_name().Cstring();

		out << YAML::Key << "widgets";
		out << YAML::Value << YAML::BeginSeq;

		for(Name_to_widget::const_iterator i = name_to_widget.begin(); i != name_to_widget.end(); ++i)
		{
			out << YAML::BeginMap;
				out << YAML::Key << "name";
				out << YAML::Value << i->second->Get_name().Cstring();
				out << YAML::Key << "prototype_name";
				out << YAML::Value << i->second->Get_prototype_name();

				i->second->To_yaml(out);
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filename.Cstring());
	fout << out.c_str();

	return true;
}

Widget* Layout::Get_root() const
{
	return root;
}

Ustring Layout::Add_widget(const Ustring& name, Widget* widget, Widget* parent)
{
	if(name == "")
		return "";
	Ustring tryname = name;
	int instance = 1;
	while(name_to_widget.find(tryname) != name_to_widget.end())
	{
		++instance;
		std::stringstream ss;
		ss<<instance;
		Ustring is;
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

void Layout::Delete_widget(const Ustring& name)
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

const Name_to_widget& Layout::Get_widgets() const
{
	return name_to_widget;
}
