#ifndef swag_slider_h
#define swag_slider_h

#include "widget.h"

class Slider: public Widget
{
public:
	Slider();
	virtual void Set_pane_position(float p) = 0;
	virtual void Set_pane_fraction(float p) = 0;
	float Get_pane_position() const;
	float Get_pane_fraction() const;
	void Set_pane_size(int s);
	int Get_pane_size() const;
protected:
	virtual void Resized();
	float pane_position;
	float pane_reference;
	int pane_size;
	int holding_pane;
};

#endif
