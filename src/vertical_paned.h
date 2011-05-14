#ifndef swag_vertical_paned_h
#define swag_vertical_paned_h

#include "container.h"

class Vertical_paned: public Container
{
public:
	Vertical_paned();
	virtual Widget* Clone() const;
	void Set_top(Widget* widget);
	void Set_bottom(Widget* widget);
	Widget* Get_top() const;
	Widget* Get_bottom() const;
	void Set_pane_position(float p);
	void Set_pane_fraction(float p);
	float Get_pane_position() const;
	void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual Widgets Get_children() const;
	virtual const Ustring& Get_tooltip(float x, float y) const;
	virtual sinxml::Element* To_xml() const;
	virtual void To_yaml(YAML::Emitter& out) const;

	enum Values
	{
		PANE
	};
private:
	virtual void Resized();
	Widget* top;
	Widget* bottom;
	float pane_position;
	float pane_reference;
	int holding_pane;
};

#endif
