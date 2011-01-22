#include <widget_view.h>

class Slider_box_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const;
	virtual void Render(const Widget& widget) const;
	virtual float Get_value(int id, const Widget& widget) const;
public:
	Widget_view* vslider_view;
	Widget_view* hslider_view;
};
