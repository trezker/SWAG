#ifndef swag_vertical_paned_h
#define swag_vertical_paned_h

#include "widget.h"

class Vertical_paned: public Widget
{
public:
	Vertical_paned();
	void Set_top(Widget* widget);
	void Set_bottom(Widget* widget);
	Widget* Get_top() const;
	Widget* Get_bottom() const;
	void Set_pane_position(float p);
	void Set_pane_fraction(float p);
	float Get_pane_position() const;
	void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
	virtual void Resized();
	Widget* top;
	Widget* bottom;
	float pane_position;
	float pane_reference;
	int holding_pane;
};

#endif