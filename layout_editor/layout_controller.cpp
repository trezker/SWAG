#include "layout_controller.h"

void Layout_controller::Set_layout(Layout* ilayout){
	layout = ilayout;
}

void Layout_controller::Set_root_tree(Tree* tree) {
	root_tree = tree;
}

bool Layout_controller::Set_tree(Tree* tree, Widget* widget) {
	treemap[tree] = widget;
}

void Layout_controller::Select_tree(Tree* tree) {
	selected_tree = tree;
}

Widget* Layout_controller::Get_widget(Tree* tree) {
	Treemap::iterator i = treemap.find(tree);
	if(i != treemap.end()) {
		return i->second;
	}
	return NULL;
}

Widget* Layout_controller::Get_current_widget() {
	return Get_widget(selected_tree);
}

Tree* Layout_controller::Get_current_tree() {
	return selected_tree;
}

void Layout_controller::Clear() {
	for(Treemap::iterator i = treemap.begin(); i != treemap.end(); ++i)
	{
		if(i->first == root_tree)
		{
			root_tree->Clear_children();
			continue;
		}
		delete i->first;
	}
	treemap.clear();
}

void Layout_controller::Destroy_widget(Tree* tree) {
	delete treemap[tree];
	treemap.erase(tree);
	delete tree;
}

bool Layout_controller::Rename_widget(Widget* widget, const Ustring& name)
{
	layout->Rename_widget(widget, name);
}
