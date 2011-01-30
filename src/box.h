#ifndef swag_box_h
#define swag_box_h

#include "container.h"

class Box: public Container
{
public:
	void Add(Widget* widget);
	const Widgets& Get_widgets() const;
	virtual void Organise() = 0;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual const std::string& Get_tooltip(float x, float y) const;
	virtual sinxml::Element* To_xml() const;
protected:
	Widgets widgets;
private:
	virtual void Handle_child_resize();
	virtual void Resized();
};

#endif
