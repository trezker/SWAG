#ifndef swag_slider_box_h
#define swag_slider_box_h

#include "container.h"
#include "horizontal_slider.h"
#include "vertical_slider.h"
#include "event_queue.h"

class Slider_box: public Container
{
public:
	Slider_box();
	Slider_box(const Slider_box& o);
	virtual Widget* Clone() const;
	void Set_child(Widget* widget);
	Widget* Get_child() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual sinxml::Element* To_xml() const;
	
	enum Values
	{
		SLIDER_WIDTH,
		SLIDER_HEIGHT
	};
	
	const Horizontal_slider& Get_hslider() const;
	const Vertical_slider& Get_vslider() const;
private:
	virtual void Resized();
	virtual void Handle_child_resize();
	Widget* child;
	Horizontal_slider hslider;
	Vertical_slider vslider;
	Event_queue slider_events;
};

#endif
