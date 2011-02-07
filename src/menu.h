#ifndef swag_menu_h
#define swag_menu_h

#include "widget.h"
#include <string>

class Menu: public Widget
{
public:
	Menu();
	virtual Widget* Clone() const;
	void Add_option(const std::string& t);
	int Get_option_count() const;
	std::string Get_option(int i) const;
	int Get_selected_option() const;
	virtual sinxml::Element* To_xml() const;
private:
	typedef std::vector<std::string> Options;
	Options options;
	int selection;
};

#endif
