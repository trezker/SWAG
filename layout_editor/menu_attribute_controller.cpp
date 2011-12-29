#include "menu_attribute_controller.h"
#include "layout_controller.h"

bool Menu_attribute_controller::Load(Skin& skin) {
	controller_layout.Set_filename("interfaces/menu.yaml");
	controller_layout.Set_skin(&skin);
	if(controller_layout.Load_yaml())
	{
		events[Event(controller_layout.Get_widget("add option"), "clicked")] = "add_option";
		events[Event(controller_layout.Get_widget("remove option"), "clicked")] = "remove_option";
		controller_layout.Get_widget("expander")->Set_event_queue(&gui_events);
		return true;
	}
	return false;
}

void Menu_attribute_controller::Set_layout_controller(Layout_controller& lc) {
	layout_controller = &lc;
}

Widget* Menu_attribute_controller::Get_root() {
	return controller_layout.Get_widget("expander");
}

void Menu_attribute_controller::Handle_event(const Ustring& event_handle, const Event& event) {
	Menu* widget = dynamic_cast<Menu*>(layout_controller->Get_current_widget());
	if(widget) {
		if(event_handle == "add_option") {
			Inputbox* option_text = dynamic_cast<Inputbox*>(controller_layout.Get_widget("option text"));
			widget->Add_option(option_text->Get_text());
		}
		if(event_handle == "remove_option") {
			widget->Remove_option(widget->Get_selected_option());
		}
	}
}

void Menu_attribute_controller::Synchronize_values() {
}

void Menu_attribute_controller::Update(){
	while(!gui_events.Empty())
	{
		const Event& gui_event = gui_events.Front();
		Process_event(gui_event);
		gui_events.Pop();
	}
}
