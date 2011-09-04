#include "editor_controller.h"
#include "layout_controller.h"
#include <allegro5/allegro_native_dialog.h>
#include <stack>

bool Editor_controller::Load(Skin& skin) {
	controller_layout.Set_filename("interfaces/editor.yaml");
	controller_layout.Set_skin(&skin);
	if(controller_layout.Load_yaml())
	{
		dynamic_cast<Container*>(controller_layout.Get_widget("layout expander"))->Add_child(layout_controller->Get_root_tree());
		events[Event(controller_layout.Get_widget("save"), "clicked")] = "save";
		events[Event(controller_layout.Get_widget("load"), "clicked")] = "load";
		events[Event(controller_layout.Get_widget("remove"), "clicked")] = "remove";
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

void Editor_controller::Handle_event(const Ustring& event_handle) {
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

			//layout.Set_filename("testlayout.yaml");
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
			//layout.Set_filename("testlayout.yaml");
			//layout.Set_skin(&skin);
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
			layout_controller->Select_tree(NULL);//layout_controller->Get_root_tree());
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
}

void Editor_controller::Synchronize_values() {
}

void Editor_controller::Set_layout_display(ALLEGRO_DISPLAY *display)
{
	layout_display = display;
}
