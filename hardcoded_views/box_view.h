#include <widget_view.h>
struct ALLEGRO_FONT;

class Box_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const;
	virtual void Render(const Widget& widget) const;
};

class HBox_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const;
	virtual void Render(const Widget& widget) const;
};
