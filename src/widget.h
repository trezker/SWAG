#ifndef swag_widget_h
#define swag_widget_h

#include <vector>
#include <string>
#include "ustring.h"

#include <allegro5/allegro.h>
#include "vector2.h"

class Widget_view;
class Event_queue;
class Event;
class Container;
class Desktop;
class Layout;

namespace YAML
{
	class Emitter;
	class Node;
}

class Widget
{
public:
	Widget();
	virtual ~Widget();
	virtual Widget* Clone() const;

	virtual void Handle_event(const ALLEGRO_EVENT& event);

	void Set_view(Widget_view* view);
//	Widget_view* Get_view() const;
	void Calculate_request_size();
	Vector2 Request_size() const;
	void Render() const;
	
	void Set_position(Vector2 position);
	void Set_size(Vector2 size);
	Vector2 Get_position() const;
	Vector2 Get_size() const;
	
	void Set_name(const Ustring& n);
	const Ustring& Get_name() const;
	void Set_prototype_name(const Ustring& n);
	const Ustring& Get_prototype_name() const;

	void Set_tooltip(const Ustring& t);
	const Ustring& Get_tooltip() const;
	virtual const Ustring& Get_tooltip(float x, float y) const;

	void Set_fixed_height(bool active);
	void Set_fixed_width(bool active);
	void Enable_fixed_height();
	void Enable_fixed_width();
	void Disable_fixed_height();
	void Disable_fixed_width();
	bool Has_fixed_height() const;
	bool Has_fixed_width() const;

	void Set_event_queue(Event_queue* event_queue);
	Event_queue* Get_event_queue() const;
	void Set_parent(Container* parent);
	Container* Get_parent() const;
	Desktop* Get_desktop();
	
	bool Covers_point(float x, float y);
	
	virtual void To_yaml(YAML::Emitter& out) const;
	virtual void From_yaml(const YAML::Node& in);
protected:	
	void Push_event(const Event& event);
	void Child_resized();
	float Get_value(int id) const;
	void Set_value(int id, float v);
private:
	virtual void Resized();
	Vector2 position;
	Vector2 size;
	Vector2 request_size;
	bool fixed_height;
	bool fixed_width;
	Widget_view* view;
	Container* parent;
	Event_queue* event_queue;
	Ustring tooltip;
	Ustring name;
	Ustring prototype_name;
};

typedef std::vector<Widget*> Widgets;

#endif
