#ifndef swag_tree_h
#define swag_tree_h

#include "container.h"
#include <string>
#include "text_interface.h"

class Tree;
typedef std::vector<Tree*> Trees;

class Tree: public Container, public Text_interface
{
public:
	Tree();
	virtual Widget* Clone() const;
	bool Is_open() const;
	void Open();
	void Close();
	virtual void Set_text(const Ustring& t);
	const Ustring& Get_text() const;

	void Add_child(Tree* c);
	void Remove_child(Tree* c);
	const Trees& Get_childtrees() const;
	Trees& Get_children();
	void Clear_children();

	bool Is_selected() const;
	void Child_selected();
	void Select();
	void Deselect();

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
	Trees children;
	bool selected;
};

#endif
