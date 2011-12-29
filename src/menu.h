#ifndef swag_menu_h
#define swag_menu_h

#include "widget.h"
#include <string>

class Menu: public Widget
{
public:
	Menu();
	virtual Widget* Clone() const;
	void Add_option(const Ustring& t);
	void Remove_option(int i);
	int Get_option_count() const;
	Ustring Get_option(int i) const;
	int Get_selected_option() const;
	virtual void To_yaml(YAML::Emitter& out) const;
private:
	typedef std::vector<Ustring> Options;
	Options options;
	int selection;
};

#endif
