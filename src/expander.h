#ifndef swag_expander_h
#define swag_expander_h

#include "container.h"
#include <string>
#include "text_interface.h"

class Expander: public Container, public Text_interface
{
public:
	Expander();
	virtual Widget* Clone() const;
	bool Is_open() const;
	void Open();
	void Close();
	virtual void Set_text(const Ustring& t);
	const Ustring& Get_text() const;

	Widget* Get_child() const;

	virtual void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual void Handle_child_resize();
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual Widgets Get_children() const;
	virtual const Ustring& Get_tooltip(float x, float y) const;
	virtual void To_yaml(YAML::Emitter& out) const;
	virtual void From_yaml(const YAML::Node& in);

	enum Value
	{
		INDENT,
		SELF_WIDTH,
		SELF_HEIGHT
	};
private:
	virtual void Resized();
	bool open;
	Ustring text;
	Widget* child;
};

#endif
