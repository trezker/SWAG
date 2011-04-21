#ifndef swag_vertical_slider_h
#define swag_vertical_slider_h

#include "slider.h"

class Vertical_slider: public Slider
{
public:
	Vertical_slider();
	virtual Widget* Clone() const;
	virtual void Set_pane_position(float p);
	virtual void Set_pane_fraction(float p);
	
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
};

#endif
