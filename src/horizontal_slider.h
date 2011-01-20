#ifndef swag_horizontal_slider_h
#define swag_horizontal_slider_h

#include "widget.h"

class Horizontal_slider: public Widget
{
public:
	Horizontal_slider();
	virtual Widget* Clone() const;
	void Set_pane_position(float p);
	void Set_pane_fraction(float p);
	float Get_pane_position() const;
	float Get_pane_fraction() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	void Set_pane_size(int s);
	int Get_pane_size() const;
private:
	virtual void Resized();
	float pane_position;
	float pane_reference;
	int pane_size;
	int holding_pane;
};

#endif
