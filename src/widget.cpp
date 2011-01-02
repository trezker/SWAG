#include "widget.h"
#include "widget_view.h"
#include "event_queue.h"
#include "container.h"

Widget::Widget()
:view(NULL)
,fixed_width(false)
,fixed_height(false)
,parent(NULL)
,event_queue(NULL)
{
}

Widget::~Widget()
{
}

Widget* Widget::Clone() const
{
	return new Widget(*this);
}

void Widget::Handle_event(const ALLEGRO_EVENT& event)
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

float Widget::Get_value(int id) const
{
	return view?view->Get_value(id, *this):0;
}

void Widget::Set_value(int id, float v)
{
	if(view)
		view->Set_value(id, v);
}

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

void Widget::Push_event(const Event& event)
{
	if(event_queue)
	{
		event_queue->Push(event);
	}
	if(parent)
	{
		parent->Push_event(event);
	}
}

void Widget::Set_event_queue(Event_queue* e)
{
	event_queue = e;
}

void Widget::Set_parent(Container* p)
{
	parent = p;
}

Container* Widget::Get_parent() const
{
	return parent;
}

bool Widget::Covers_point(float x, float y)
{
	Vector2 s = Get_size();
	Vector2 p = Get_position();
	return (x > p.x && y > p.y+1
		&& x < p.x+s.x-1 && y < p.y + s.y);
}

void Widget::Child_resized()
{
	if(parent)
		parent->Handle_child_resize();
}
