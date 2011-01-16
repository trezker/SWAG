#include <widget_view.h>
struct ALLEGRO_FONT;

class Inputbox_view: public Widget_view
{
public:
	Inputbox_view();
	virtual Vector2 Request_size(const Widget& widget) const;
	virtual float Get_value(int id, const Widget& widget) const;
	virtual void Set_value(int id, float v);
	virtual void Render(const Widget& widget) const;
	virtual void Update(float t);
public:
	ALLEGRO_FONT* font;
	float cursor_flash;
};
