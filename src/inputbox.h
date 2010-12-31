#ifndef swag_inputbox_h
#define swag_inputbox_h

#include "widget.h"
#include <string>

class Inputbox: public Widget
{
public:
	Inputbox();
	virtual ~Inputbox();
	Inputbox(const Inputbox& o);
	virtual Widget* Clone() const;
	void Set_text(const std::string& t);
	std::string Get_text() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	bool Is_pressed() const;
	bool Has_focus() const;
private:
	ALLEGRO_USTR *text;
	bool pressed;
	bool has_focus;
	bool mouse_over;
};

#endif
