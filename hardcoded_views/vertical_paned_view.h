#include <widget_view.h>
struct ALLEGRO_FONT;

class Vertical_paned_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const;
	virtual float Get_value(int id, const Widget& widget) const;
	virtual void Render(const Widget& widget) const;
};
