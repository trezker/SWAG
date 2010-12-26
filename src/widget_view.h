#ifndef swag_widget_view_h
#define swag_widget_view_h

#include "vector2.h"
class Widget;

class Widget_view
{
public:
	virtual ~Widget_view(){}
	virtual Vector2 Request_size(const Widget& widget) const = 0;
	virtual void Render(const Widget& widget) const = 0;
private:
};

#endif
