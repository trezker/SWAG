#ifndef swag_tree_h
#define swag_tree_h

#include "container.h"
#include <string>

class Tree;
typedef std::vector<Tree*> Trees;

class Tree: public Container
{
public:
	Tree();
	virtual Widget* Clone() const;
	bool Is_open() const;
	void Open();
	void Close();
	void Set_text(const std::string& t);
	const std::string& Get_text() const;

	void Add_child(Tree* c);
	void Remove_child(Tree* c);
	const Trees& Get_children() const;
	Trees& Get_children();

	bool Is_selected() const;
	void Child_selected();
	void Select();
	void Deselect();

	virtual void Organise();
	virtual void Handle_event(const ALLEGRO_EVENT& event);
	virtual void Handle_child_resize();
	virtual bool Add_child(Widget* c);
	virtual void Remove_child(Widget* c);
	virtual const std::string& Get_tooltip(float x, float y) const;

	enum Value
	{
		INDENT,
		SELF_WIDTH,
		SELF_HEIGHT
	};
private:
	virtual void Resized();
	bool open;
	std::string text;
	Trees children;
	bool selected;
};

#endif
