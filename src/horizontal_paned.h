#ifndef swag_horizontal_paned_h
#define swag_horizontal_paned_h

#include "widget.h"

class Horizontal_paned: public Widget
{
public:
	Horizontal_paned();
	void Set_left(Widget* widget);
	void Set_right(Widget* widget);
	Widget* Get_left() const;
	Widget* Get_right() const;
	void Set_pane_position(float p);
	void Set_pane_fraction(float p);
	float Get_pane_position() const;
	void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
	virtual void Resized();
	Widget* left;
	Widget* right;
	float pane_position;
	float pane_reference;
	int holding_pane;
};

#endif
