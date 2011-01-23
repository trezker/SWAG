#ifndef swag_inputbox_h
#define swag_inputbox_h

#include "widget.h"
#include <string>
#include "text_interface.h"

class Inputbox: public Widget, public Text_interface
{
public:
	Inputbox();
	virtual ~Inputbox();
	Inputbox(const Inputbox& o);
	virtual Widget* Clone() const;
	virtual void Set_text(const std::string& t);
	std::string Get_text() const;
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	bool Is_pressed() const;
	bool Has_focus() const;
	int Cursor_position() const;
	int Get_selection_start() const;
	int Get_selection_end() const;
	
	enum Value
	{
		POSITION,
		FLASH
	};
private:
	bool Remove_range();
	ALLEGRO_USTR *text;
	bool pressed;
	bool has_focus;
	bool mouse_over;
	int cursor;
	int selection_start;
	int selection_end;
};

#endif
