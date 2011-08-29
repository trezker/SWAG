#ifndef widget_attribute_controller_h
#define widget_attribute_controller_h

#include "controller.h"

class Layout_controller;

class Widget_attribute_controller: public Controller {
public:
	virtual bool Load(Skin& skin);
	void Set_layout_controller(Layout_controller& lc);
	virtual Widget* Get_root();
	virtual void Handle_event(const Ustring& event_handle);
	virtual void Synchronize_values();
private:
	Layout_controller* layout_controller;
};

#endif
