#ifndef editor_controller_h
#define editor_controller_h

#include "controller.h"

class Layout_controller;

class Editor_controller: public Controller {
public:
	virtual bool Load(Skin& skin);
	void Set_layout_controller(Layout_controller& lc);
	virtual Widget* Get_root();
	virtual void Handle_event(const Ustring& event_handle);
	virtual void Synchronize_values();
	void Set_layout_display(ALLEGRO_DISPLAY *display);
private:
	Layout_controller* layout_controller;
	ALLEGRO_DISPLAY *layout_display;
};

#endif
