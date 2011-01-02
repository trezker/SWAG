#ifndef swag_widget_view_h
#define swag_widget_view_h

#include "vector2.h"
class Widget;

class Widget_view
{
public:
	virtual ~Widget_view(){}
	virtual Vector2 Request_size(const Widget& widget) const = 0;
	virtual float Get_value(int id, const Widget& widget) const {return 0;}
	virtual void Set_value(int id, float v) {}
	virtual void Render(const Widget& widget) const = 0;
	virtual void Update(float t){}
private:
};

#endif
