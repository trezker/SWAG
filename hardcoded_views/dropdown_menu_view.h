#include <widget_view.h>
struct ALLEGRO_FONT;

class Dropdown_menu_view: public Widget_view
{
public:
	Vector2 Request_size(const Widget& widget) const;
	float Get_value(int id, const Widget& widget) const;
	void Render(const Widget& widget) const;
public:
	ALLEGRO_FONT* font;
};
