#ifndef layout_controller_h
#define layout_controller_h

#include <map>
#include "swag.h"

typedef std::map<Tree*, Widget*> Treemap;

class Layout_controller {
public:
	void Set_layout(Layout* layout);
	void Set_root_tree(Tree* tree);
	bool Set_tree(Tree* tree, Widget* widget);
	void Select_tree(Tree* tree);
	Widget* Get_widget(Tree* tree);
	Widget* Get_current_widget();
	Tree* Get_current_tree();
	void Clear();
	void Destroy_widget(Tree* tree);
	bool Rename_widget(Widget* widget, const Ustring& name);
private:
	Tree* root_tree;
	Treemap treemap;
	Tree* selected_tree;
	Layout* layout;
};

#endif
