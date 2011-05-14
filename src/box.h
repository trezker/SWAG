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
	virtual Widgets Get_children() const;
	virtual const Ustring& Get_tooltip(float x, float y) const;
	virtual sinxml::Element* To_xml() const;
	virtual void To_yaml(YAML::Emitter& out) const;
protected:
	Widgets widgets;
private:
	virtual void Handle_child_resize();
	virtual void Resized();
};

#endif
