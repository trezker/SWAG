#ifndef button_attribute_controller_h
#define button_attribute_controller_h

#include "controller.h"

class Layout_controller;

class Button_attribute_controller: public Controller {
public:
	virtual bool Load(Skin& skin);
	void Set_layout_controller(Layout_controller& lc);
	virtual Widget* Get_root();
	virtual void Handle_event(const Ustring& event_handle);
private:
	Layout_controller* layout_controller;
};

#endif
