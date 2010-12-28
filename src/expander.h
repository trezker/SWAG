#ifndef swag_expander_h
#define swag_expander_h

#include "container.h"
#include <string>

class Expander;
typedef std::vector<Expander*> Expanders;

class Expander: public Container
{
public:
	Expander();
	bool Is_open() const;
	void Open();
	void Close();
	void Set_text(const std::string& t);
	const std::string& Get_text() const;

	void Add_child(Expander* c);
	const Expanders& Get_children() const;
	virtual void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual void Handle_child_resize();
private:
	virtual void Resized();
	bool open;
	std::string text;
	Expanders children;
};

#endif
