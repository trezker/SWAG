#include "layout.h"
#include "widget.h"
#include "skin.h"
#include <sstream>
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

	try
	{
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
			Ustring prototype_name;
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
			w->From_yaml(doc["widgets"][i]);
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
	}
	catch(YAML::Exception& e){
		std::cout<<"ERROR: "<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Caught error"<<std::endl;
		root = NULL;
	}
	if(!root)
	{
		Clear();
		return false;
	}
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

Widget* Layout::Get_widget(const Ustring& name)
{
	Name_to_widget::iterator i = name_to_widget.find(name);
	if(i != name_to_widget.end()) {
		return i->second;
	} else {
		return NULL;
	}
}
