#ifndef swag_button_h
#define swag_button_h

#include "widget.h"
#include <string>

class Button: public Widget
{
public:
	void Set_text(const std::string& t);
	const std::string& Get_text() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
private:
	std::string text;
	bool pressed;
	bool mouse_over;
};

#endif
