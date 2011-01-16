#include "label.h"

Label::Label()
{
	Enable_fixed_width();
	Enable_fixed_height();
}

Widget* Label::Clone() const
{
	return new Label(*this);
}

void Label::Set_text(const std::string& t)
{
	text = t;
	Child_resized();
}

std::string Label::Get_text() const
{
	return text;
}
