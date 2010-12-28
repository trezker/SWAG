#ifndef swag_layout_h
#define swag_layout_h

#include "container.h"

class Layout: public Container
{
public:
	void Add(Widget* widget);
	const Widgets& Get_widgets() const;
	virtual void Organise() = 0;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
protected:
	Widgets widgets;
private:
	virtual void Resized();
};

#endif
