#include "widget.h"
#include "widget_view.h"

Widget::Widget()
:view(NULL)
,fixed_width(false)
,fixed_height(false)
{
}

Widget::~Widget()
{
}

void Widget::Event(const ALLEGRO_EVENT& event)
{
}

void Widget::Set_view(Widget_view* v)
{
	view = v;
}
/*
Widget_view* Widget::Get_view() const
{
	return view;
}
*/
Vector2 Widget::Request_size() const
{
	if(view)
		return view->Request_size(*this);
	return Vector2();
}

void Widget::Render() const
{
	if(view)
		view->Render(*this);
}

void Widget::Set_position(Vector2 p)
{
	position = p;
	Resized();
}

void Widget::Set_size(Vector2 s)
{
	size = s;
	Resized();
}

Vector2 Widget::Get_position() const
{
	return position;
}

Vector2 Widget::Get_size() const
{
	return size;
}

void Widget::Enable_fixed_height()
{
	fixed_height = true;
}

void Widget::Enable_fixed_width()
{
	fixed_width = true;
}

void Widget::Disable_fixed_height()
{
	fixed_height = false;
}

void Widget::Disable_fixed_width()
{
	fixed_width = false;
}

bool Widget::Has_fixed_height() const
{
	return fixed_height;
}

bool Widget::Has_fixed_width() const
{
	return fixed_width;
}

void Widget::Resized()
{
}
