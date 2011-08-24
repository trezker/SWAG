#ifndef swag_desktop_h
#define swag_desktop_h

#include "container.h"

class Desktop: public Container
{
public:
	Desktop();
	virtual Widget* Clone() const;
	void Set_child(Widget* widget);
	Widget* Get_child() const;
	Vector2 Get_tooltip_position() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual Widgets Get_children() const;
	virtual void To_yaml(YAML::Emitter& out) const;
private:
	void Reset_tooltip();
	virtual void Resized();
	Widget* child;
	Vector2 tooltip_position;
	float tooltip_countdown;
};

#endif
