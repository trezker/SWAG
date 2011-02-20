#include "skin.h"
#include "widget_view.h"

Skin::~Skin()
{
	for(Prototypes::iterator i = prototypes.begin(); i != prototypes.end(); ++i)
	{
		delete i->second;
	}
	for(Views::iterator i = views.begin(); i != views.end(); ++i)
		delete *i;
}

void Skin::Set_prototype(const std::string& name, Widget* prototype)
{
	Prototypes::iterator i = prototypes.find(name);
	if(i != prototypes.end())
	{
		delete i->second;
	}
	prototype->Set_prototype_name(name);
	prototypes[name] = prototype;
}

void Skin::Remove_prototype(const std::string& name)
{
	Prototypes::iterator i = prototypes.find(name);
	if(i != prototypes.end())
	{
		delete i->second;
		prototypes.erase(i);
	}
}

void Skin::Add_view(Widget_view* view)
{
	views.push_back(view);
}

void Skin::Add_animated_view(Widget_view* view)
{
	animated_views.push_back(view);
}

void Skin::Update(float t)
{
	for(Views::iterator i = animated_views.begin(); i != animated_views.end(); ++i)
	{
		(*i)->Update(t);
	}
}

Namelist Skin::Get_prototype_list() const
{
	Namelist l;
	for(Prototypes::const_iterator i = prototypes.begin(); i != prototypes.end(); ++i)
	{
		l.push_back(i->first);
	}
	return l;
}
