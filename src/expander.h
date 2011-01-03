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
	virtual Widget* Clone() const;
	bool Is_open() const;
	void Open();
	void Close();
	void Set_text(const std::string& t);
	const std::string& Get_text() const;

	void Add_child(Expander* c);
	void Remove_child(Expander* c);
	const Expanders& Get_children() const;
	Expanders& Get_children();

	bool Is_selected() const;
	void Child_selected();
	void Deselect();

	virtual void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual void Handle_child_resize();
	virtual bool Add_child(Widget* c);

	enum Value
	{
		INDENT
	};
private:
	virtual void Resized();
	bool open;
	std::string text;
	Expanders children;
	bool selected;
};

#endif
