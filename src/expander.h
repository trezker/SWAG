#ifndef swag_expander_h
#define swag_expander_h

#include "layout.h"
#include <string>

class Expander: public Layout
{
public:
	Expander();
	bool Is_open() const;
	void Open();
	void Close();
	void Set_text(const std::string& t);
	const std::string& Get_text() const;
	virtual void Organise();
	virtual void Event(const ALLEGRO_EVENT& event);
private:
	bool open;
	std::string text;
};

#endif
