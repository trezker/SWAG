#include "tree.h"
#include "widget_view.h"
#include "event_queue.h"
#include <algorithm>
#include <yaml-cpp/yaml.h>

Tree::Tree()
:open(false)
,selected(false)
{
	Enable_fixed_height();
	Enable_fixed_width();
}

Widget* Tree::Clone() const
{
	return new Tree(*this);
}

bool Tree::Is_open() const
{
	return open;
}

void Tree::Open()
{
	if(!open)
	{
		open = true;
		Child_resized();
	}
}

void Tree::Close()
{
	if(open)
	{
		open = false;
		Child_resized();
	}
}

void Tree::Set_text(const Ustring& t)
{
	text = t;
}

const Ustring& Tree::Get_text() const
{
	return text;
}

void Tree::Add_child(Tree* c)
{
	if(!c)
		return;
	children.push_back(c);
	c->Set_parent(this);
	Organise();
	Child_resized();
}

void Tree::Remove_child(Tree* c)
{
	Trees::iterator i = std::find(children.begin(), children.end(), c);
	if(i!=children.end())
		children.erase(i);
	Organise();
	Child_resized();
}

const Trees& Tree::Get_childtrees() const
{
	return children;
}

Trees& Tree::Get_children()
{
	return children;
}

void Tree::Clear_children()
{
	children.clear();
}

bool Tree::Is_selected() const
{
	return selected;
}

void Tree::Child_selected()
{
	Tree* parent = dynamic_cast<Tree*>(Get_parent());
	if(parent)
		parent->Child_selected();
	Deselect();
}

void Tree::Select()
{
	Child_selected();
	selected = true;
	Push_event(Event(this, "selected"));
}

void Tree::Deselect()
{
	bool event_deselected = selected;
	selected = false;
	for(Trees::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Deselect();
	}
	if(event_deselected)
		Push_event(Event(this, "deselected"));
}

void Tree::Organise()
{
	float indent = Get_value(INDENT);
	float y = Get_position().y;
	float x = Get_position().x + indent;
	y += Get_value(SELF_HEIGHT);

	for(Trees::iterator i = children.begin(); i != children.end(); ++i)
	{
		Vector2 size = (*i)->Request_size();
		(*i)->Set_size(size);
		(*i)->Set_position(Vector2(x, y));
		y += size.y;
	}
}

void Tree::Handle_event(const ALLEGRO_EVENT& event)
{
	if(Is_open())
	{
		for(Trees::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Handle_event(event);
		}
	}

	if(ALLEGRO_EVENT_MOUSE_BUTTON_UP == event.type)
	{
		Vector2 p = Get_position();
		bool covers_point = Covers_point(event.mouse.x, event.mouse.y);

		if(covers_point && event.mouse.y < p.y+Get_value(SELF_HEIGHT))
		{
			if(event.mouse.x < p.x+Get_value(INDENT))
			{
				if(Is_open())
					Close();
				else
					Open();
				Child_resized();
			}
			else
			{
				Child_selected();
				selected = true;
				Push_event(Event(this, "selected"));
			}
		}
	}
}

void Tree::Resized()
{
	Organise();
}

void Tree::Handle_child_resize()
{
	Resized();
	Child_resized();
}

bool Tree::Add_child(Widget* c)
{
	Tree* e = dynamic_cast<Tree*>(c);
	if(e)
	{
		Add_child(e);
		return true;
	}
	return false;
}

void Tree::Remove_child(Widget* c)
{
	Trees::iterator i = std::find(children.begin(), children.end(), c);
	if(i != children.end())
	{
		children.erase(i);
	}
	Organise();
}

Widgets Tree::Get_children() const
{
	Widgets w;
	for(Trees::const_iterator i = children.begin(); i != children.end(); ++i)
	{
		w.push_back(*i);
	}
	return w;
}

const Ustring& Tree::Get_tooltip(float x, float y) const
{
	for(Trees::const_iterator i = children.begin(); i != children.end(); ++i)
	{
		if((*i)->Covers_point(x, y))
			return (*i)->Get_tooltip(x, y);
	}
	return Widget::Get_tooltip(x, y);
}

void Tree::To_yaml(YAML::Emitter& out) const
{
	Container::To_yaml(out);
	out << YAML::Key << "Tree";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "text";
		out << YAML::Value << text.Cstring();
	out << YAML::EndMap;

	out << YAML::Key << "children";
	out << YAML::Value << YAML::BeginSeq;
	for(Trees::const_iterator i = children.begin(); i != children.end(); ++i)
	{
		const Ustring &child_name = (*i)->Get_name();
		if(child_name != "")
		{
			out << child_name;
		}
	}

	out << YAML::EndSeq;
}

void Tree::From_yaml(const YAML::Node& in)
{
	Widget::From_yaml(in);
	const YAML::Node& doc = in["Tree"];
	doc["text"] >> text;
	std::cout<<"Loaded Tree: "<<text<<std::endl;
}
