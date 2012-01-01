#ifndef swag_dropdown_menu_h
#define swag_dropdown_menu_h

#include "container.h"
#include <string>

class Dropdown_menu: public Container
{
public:
	Dropdown_menu();
	virtual Widget* Clone() const;

	bool Is_open() const;
	void Open();
	void Close();

	Widget* Get_child() const;
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual Widgets Get_children() const;
	virtual void Close_child(Widget* c);
	virtual void Organise();

	virtual void To_yaml(YAML::Emitter& out) const;
	virtual void From_yaml(const YAML::Node& in);
	virtual void Handle_event(const ALLEGRO_EVENT& event);

	enum Value
	{
		SELF_HEIGHT
	};
private:
	bool open;
	Widget* child;
};

#endif
