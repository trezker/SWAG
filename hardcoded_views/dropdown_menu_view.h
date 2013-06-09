#include <widget_view.h>
class Font;

class Dropdown_menu_view: public Widget_view
{
public:
	Vector2 Request_size(const Widget& widget) const;
	float Get_value(int id, const Widget& widget) const;
	void Render(const Widget& widget) const;
public:
	Font* font;
};
