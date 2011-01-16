#include "menu.h"

Menu::Menu()
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Menu::Clone() const
{
	return new Menu(*this);
}

void Menu::Add_option(const std::string& t)
{
	options.push_back(t);
}

int Menu::Get_option_count() const
{
	return options.size();
}

std::string Menu::Get_option(int i) const
{
	if(i < options.size())
		return options[i];
	return "";
}

int Menu::Get_selected_option() const
{
	return selection;
}
