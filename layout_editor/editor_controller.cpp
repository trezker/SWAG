#include "editor_controller.h"
#include "layout_controller.h"
#include <allegro5/allegro_native_dialog.h>
#include <stack>
#include <algorithm>
#include "button_attribute_controller.h"
#include "widget_attribute_controller.h"
#include "menu_attribute_controller.h"

bool Editor_controller::Load(Skin& skin) {
	controller_layout.Set_filename("interfaces/editor.yaml");
	controller_layout.Set_skin(&skin);
	if(controller_layout.Load_yaml())
	{
		Namelist protlist = skin.Get_prototype_list();

		Menu* create_menu = dynamic_cast<Menu*>(controller_layout.Get_widget("create menu"));
		std::cout<<"THE CREATE MENU: "<<create_menu<<std::endl;
		for(Namelist::iterator i = protlist.begin(); i != protlist.end(); ++i)
		{
			create_menu->Add_option(*i);
		}
		events[Event(controller_layout.Get_widget("create button"), "clicked")] = "create";

		//Widget attribute editing interfaces
		Widget_attribute_controller* widget_controller = new Widget_attribute_controller;
		widget_controller->Load(layout_controller->Get_skin());
		widget_controller->Set_layout_controller(*layout_controller);
		attribute_controllers["widget"] = widget_controller;

		Button_attribute_controller* button_controller = new Button_attribute_controller;
		button_controller->Load(layout_controller->Get_skin());
		button_controller->Set_layout_controller(*layout_controller);
		attribute_controllers["button"] = button_controller;

		Menu_attribute_controller* menu_controller = new Menu_attribute_controller;
		menu_controller->Load(layout_controller->Get_skin());
		menu_controller->Set_layout_controller(*layout_controller);
		attribute_controllers["menu"] = menu_controller;

		dynamic_cast<Container*>(controller_layout.Get_widget("layout expander"))->Add_child(layout_controller->Get_root_tree());
		events[Event(controller_layout.Get_widget("save"), "clicked")] = "save";
		events[Event(controller_layout.Get_widget("load"), "clicked")] = "load";
		events[Event(controller_layout.Get_widget("remove"), "clicked")] = "remove";
		events[Event(controller_layout.Get_widget("cut"), "clicked")] = "cut";
		events[Event(controller_layout.Get_widget("paste"), "clicked")] = "paste";
		events[Event(controller_layout.Get_widget("move up"), "clicked")] = "move up";
		events[Event(controller_layout.Get_widget("move down"), "clicked")] = "move down";

		controller_layout.Get_widget("vertical box")->Set_event_queue(&gui_events);
		return true;
	}
	return false;
}

void Editor_controller::Set_layout_controller(Layout_controller& lc) {
	layout_controller = &lc;
}

Widget* Editor_controller::Get_root() {
	return controller_layout.Get_widget("vertical box");
}

void Editor_controller::Handle_event(const Ustring& event_handle, const Event& event) {
	Layout& layout = layout_controller->Get_layout();
	if(event_handle == "save") {
		ALLEGRO_FILECHOOSER* fc = al_create_native_file_dialog(
			layout.Get_filename().Cstring(),
			"Save",
			"*",
			ALLEGRO_FILECHOOSER_SAVE);
		al_show_native_file_dialog(NULL, fc);
		if(al_get_native_file_dialog_count(fc) > 0) {
			const char *filename = al_get_native_file_dialog_path(fc, 0);
			layout.Set_filename(filename);

			bool s = layout.Save_yaml();
			std::cout<<(s?"Saved":"Save failed")<<std::endl;
		}
		al_destroy_native_file_dialog(fc);
	}
	if(event_handle == "load") {
		ALLEGRO_FILECHOOSER* fc = al_create_native_file_dialog(
			layout.Get_filename().Cstring(),
			"Load",
			"*",
			ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
		al_show_native_file_dialog(NULL, fc);
		if(al_get_native_file_dialog_count(fc) > 0) {
			const char *filename = al_get_native_file_dialog_path(fc, 0);
			layout.Set_filename(filename);

			layout_controller->Clear();
			if(layout.Load_yaml())
			{
				std::cout<<"Loaded"<<std::endl;
				const Name_to_widget& layout_widgets = layout.Get_widgets();

				layout_controller->Set_root(layout.Get_root());
				layout_controller->Get_root()->Set_size(Vector2(al_get_display_width(layout_display), al_get_display_height(layout_display)));
				layout_controller->Set_tree(layout_controller->Get_root_tree(), layout.Get_root());

				typedef std::stack<Tree*> Trees_todo;
				Trees_todo trees_todo;
				trees_todo.push(layout_controller->Get_root_tree());
				
				while(!trees_todo.empty())
				{
					Tree* current_tree = trees_todo.top();
					trees_todo.pop();

					Container* parent = dynamic_cast<Container*>(layout_controller->Get_widget(current_tree));

					if(parent)
					{
						Widgets children = parent->Get_children();
						for(Widgets::iterator i = children.begin(); i != children.end(); ++i)
						{
							Tree* tree_child = layout_controller->Get_skin().Clone<Tree>("tree");
							tree_child->Set_text((*i)->Get_name());
							layout_controller->Set_tree(tree_child, *i);
							current_tree->Add_child(tree_child);
							events[Event(tree_child, "selected")] = "select";
							trees_todo.push(tree_child);
						}
					}
				}
			}
			else
			{
				layout_controller->Set_root(layout_controller->Get_skin().Clone<Desktop>("desktop"));
				layout_controller->Get_root()->Set_position(Vector2(0, 0));
				layout_controller->Get_root()->Set_size(Vector2(al_get_display_width(layout_display), al_get_display_height(layout_display)));
				
				layout.Add_widget("root", layout_controller->Get_root(), NULL);
				layout_controller->Set_tree(layout_controller->Get_root_tree(), layout_controller->Get_root());
			}
			layout_controller->Select_tree(NULL);
		}
		al_destroy_native_file_dialog(fc);
	}
	if(event_handle == "remove") {
		Tree* parent = dynamic_cast<Tree*>(layout_controller->Get_current_tree()->Get_parent());
		if(parent)
		{
			dynamic_cast<Container*>(layout_controller->Get_widget(parent))->Remove_child(layout_controller->Get_current_widget());

			parent->Remove_child(layout_controller->Get_current_tree());
			Trees deadlist;
			deadlist.push_back(layout_controller->Get_current_tree());
			layout_controller->Select_tree(NULL);
			int count = 0;
			while(!deadlist.empty())
			{
				Tree* current = deadlist.back();
				deadlist.erase(--deadlist.end());
				Trees& children = current->Get_children();
				for(Trees::iterator i = children.begin(); i != children.end(); ++i)
				{
					deadlist.push_back(*i);
				}
				layout.Remove_widget(layout_controller->Get_widget(current));
				layout_controller->Destroy_widget(current);
				++count;
			}
			layout_controller->Select_tree(parent);
			parent->Select();
		}
	}
	if(event_handle == "create") {
		std::cout<<"Create"<<std::endl;
		if(layout_controller->Get_current_tree())
		{
			std::cout<<"Current tree"<<std::endl;
			Menu* create_menu = dynamic_cast<Menu*>(controller_layout.Get_widget("create menu"));
			int option = create_menu->Get_selected_option();
			if(option != -1){
				Widget* child = layout_controller->Get_skin().Clone<Widget>(create_menu->Get_option(option));
				Container* parent = dynamic_cast<Container*>(layout_controller->Get_current_widget());
				std::cout<<"parent: "<<parent<<std::endl;

				if(parent && parent->Add_child(child))
				{
					Ustring name = layout.Add_widget(create_menu->Get_option(option), child, parent);
					Tree* tree_child = layout_controller->Get_skin().Clone<Tree>("tree");
					tree_child->Set_text(name);
					layout_controller->Get_current_tree()->Add_child(tree_child);
					layout_controller->Set_tree(tree_child, child);
					layout_controller->Get_current_tree()->Open();
					Text_interface* has_text = dynamic_cast<Text_interface*>(child);
					if(has_text)
						has_text->Set_text(name);
					events[Event(tree_child, "selected")] = "select";
				}
				else
					delete child;
			}
		}
	}

	if(event_handle == "select")
	{
		Tree* newsel = dynamic_cast<Tree*>(event.source);
		if(newsel)
		{
			Widget* tw = layout_controller->Get_widget(newsel);
			if(tw)
			{
				Container* attributes_vbox = dynamic_cast<Container*>(controller_layout.Get_widget("attribute vbox"));
				layout_controller->Select_tree(newsel);
				Widgets widgets = attributes_vbox->Get_children();
				for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i) {
					attributes_vbox->Remove_child(*i);
				}
				attributes_vbox->Add_child(attribute_controllers["widget"]->Get_root());
				attribute_controllers["widget"]->Synchronize_values();
				Attribute_controllers::iterator ac = attribute_controllers.find(tw->Get_prototype_name().Cstring());
				if(ac != attribute_controllers.end()) {
					attributes_vbox->Add_child(ac->second->Get_root());
					ac->second->Synchronize_values();
				}
			}
		}
	}
	if(event_handle == "move up") {
		Tree* currenttree = dynamic_cast<Tree*>(layout_controller->Get_current_tree());
		Tree* parenttree = dynamic_cast<Tree*>(currenttree->Get_parent());
		if(currenttree && parenttree)
		{
			Widget* widget = layout_controller->Get_current_widget();
			Container* parent = dynamic_cast<Container*>(widget->Get_parent());
			Widgets children = parent->Get_children();
			Widgets::iterator i = std::find(children.begin(), children.end(), widget);
			Trees treechildren = parenttree->Get_children();
			Trees::iterator itree = std::find(treechildren.begin(), treechildren.end(), currenttree);
			if(i != children.begin()) {
				i = children.erase(i);
				itree = treechildren.erase(itree);
				if(i != children.begin()){
					--i;
					--itree;
				}
				children.insert(i, widget);
				treechildren.insert(itree, currenttree);
				for(Widgets::iterator i = children.begin(); i != children.end(); ++i) {
					parent->Remove_child(*i);
				}
				for(Trees::iterator i = treechildren.begin(); i != treechildren.end(); ++i) {
					parenttree->Remove_child(*i);
				}
				for(Widgets::iterator i = children.begin(); i != children.end(); ++i) {
					parent->Add_child(*i);
				}
				for(Trees::iterator i = treechildren.begin(); i != treechildren.end(); ++i) {
					parenttree->Add_child(*i);
				}
			}
		}
	}
	if(event_handle == "move down") {
		Tree* currenttree = dynamic_cast<Tree*>(layout_controller->Get_current_tree());
		Tree* parenttree = dynamic_cast<Tree*>(currenttree->Get_parent());
		if(currenttree && parenttree)
		{
			Widget* widget = layout_controller->Get_current_widget();
			Container* parent = dynamic_cast<Container*>(widget->Get_parent());
		}
	}
	if(event_handle == "cut") {
		Tree* currenttree = dynamic_cast<Tree*>(layout_controller->Get_current_tree());
		Tree* parenttree = dynamic_cast<Tree*>(currenttree->Get_parent());
		if(currenttree && parenttree)
		{
			Widget* widget = layout_controller->Get_current_widget();
			Container* parent = dynamic_cast<Container*>(widget->Get_parent());
		}
	}
	if(event_handle == "paste") {
		Tree* currenttree = dynamic_cast<Tree*>(layout_controller->Get_current_tree());
		Tree* parenttree = dynamic_cast<Tree*>(currenttree->Get_parent());
		if(currenttree && parenttree)
		{
			Widget* widget = layout_controller->Get_current_widget();
			Container* parent = dynamic_cast<Container*>(widget->Get_parent());
		}
	}
}

void Editor_controller::Synchronize_values() {
}

void Editor_controller::Set_layout_display(ALLEGRO_DISPLAY *display)
{
	layout_display = display;
}

void Editor_controller::Update()
{
	while(!gui_events.Empty())
	{
		const Event& gui_event = gui_events.Front();
		Process_event(gui_event);
		gui_events.Pop();
	}
	for(Attribute_controllers::iterator i = attribute_controllers.begin(); i != attribute_controllers.end(); ++i){
		i->second->Update();
	}
}
