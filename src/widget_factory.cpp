#include "widget_factory.h"

Widget_factory::~Widget_factory()
{
	for(Prototypes::iterator i = prototypes.begin(); i != prototypes.end(); ++i)
	{
		delete i->second;
	}
}

void Widget_factory::Set_prototype(const std::string& name, Widget* prototype)
{
	Prototypes::iterator i = prototypes.find(name);
	if(i != prototypes.end())
	{
		delete i->second;
	}
	prototypes[name] = prototype;
}

void Widget_factory::Remove_prototype(const std::string& name)
{
	Prototypes::iterator i = prototypes.find(name);
	if(i != prototypes.end())
	{
		delete i->second;
		prototypes.erase(i);
	}
}
