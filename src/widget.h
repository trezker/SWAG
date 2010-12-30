#ifndef swag_widget_h
#define swag_widget_h

#include <vector>

#include <allegro5/allegro.h>
#include "vector2.h"
class Widget_view;
class Event_queue;
class Event;
class Container;

class Widget
{
public:
	Widget();
	virtual ~Widget();

	virtual void Handle_event(const ALLEGRO_EVENT& event);

	void Set_view(Widget_view* view);
//	Widget_view* Get_view() const;
	Vector2 Request_size() const;
	void Render() const;
	
	void Set_position(Vector2 position);
	void Set_size(Vector2 size);
	Vector2 Get_position() const;
	Vector2 Get_size() const;

	void Enable_fixed_height();
	void Enable_fixed_width();
	void Disable_fixed_height();
	void Disable_fixed_width();
	bool Has_fixed_height() const;
	bool Has_fixed_width() const;

	void Set_event_queue(Event_queue* event_queue);
	void Set_parent(Container* parent);
	Container* Get_parent() const;
	
	bool Covers_point(float x, float y);
protected:	
	void Push_event(const Event& event);
	void Child_resized();
private:
	virtual void Resized();
	Vector2 position;
	Vector2 size;
	bool fixed_height;
	bool fixed_width;
	Widget_view* view;
	Container* parent;
	Event_queue* event_queue;
};

typedef std::vector<Widget*> Widgets;

#endif
