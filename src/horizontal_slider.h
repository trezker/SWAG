#ifndef swag_horizontal_slider_h
#define swag_horizontal_slider_h

#include "slider.h"

class Horizontal_slider: public Slider
{
public:
	Horizontal_slider();
	virtual Widget* Clone() const;
	void Set_pane_position(float p);
	void Set_pane_fraction(float p);
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
};

#endif
