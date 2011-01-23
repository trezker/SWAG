#ifndef swag_button_h
#define swag_button_h

#include "widget.h"
#include <string>
#include "text_interface.h"

class Button: public Widget, public Text_interface
{
public:
	Button();
	virtual Widget* Clone() const;
	virtual void Set_text(const std::string& t);
	const std::string& Get_text() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	bool Is_pressed() const;
	bool Is_mouse_over() const;
	void Set_toggle(bool t);
	bool Is_toggle() const;
	void Activate();
	void Deactivate();
	bool Is_active() const;
private:
	std::string text;
	bool pressed;
	bool mouse_over;
	bool toggle;
	bool active;
};

#endif
