#ifndef swag_vertical_slider_h
#define swag_vertical_slider_h

#include "widget.h"

class Vertical_slider: public Widget
{
public:
	Vertical_slider();
	virtual Widget* Clone() const;
	void Set_pane_position(float p);
	void Set_pane_fraction(float p);
	float Get_pane_position() const;
	void Set_pane_size(int s);
	int Get_pane_size() const;
	
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
	virtual void Resized();
	Widget* top;
	Widget* bottom;
	float pane_position;
	float pane_reference;
	int pane_size;
	int holding_pane;
};

#endif
