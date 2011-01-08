#include "label.h"

Widget* Label::Clone() const
{
	return new Label(*this);
}

void Label::Set_text(const std::string& t)
{
	text = t;
}

std::string Label::Get_text() const
{
	return text;
}
