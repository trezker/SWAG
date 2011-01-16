#ifndef swag_context_menu_h
#define swag_context_menu_h

#include "container.h"
#include <string>

class Menu;

class Context_menu: public Container
{
public:
	Context_menu();
	virtual Widget* Clone() const;
	void Set_context(Widget* widget);
	Widget* Get_context() const;
	void Set_text(const std::string& t);
	std::string Get_text() const;
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
private:
	Widget* context;
	std::string text;
	//Menu is a separate class that holds the menu structure and info for rendering
	//Since context menu is only a container for the context.
	//The menu class can also be used for popping up from a Menu_bar
	Menu* menu;
};

#endif
