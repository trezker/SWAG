#include "button_attribute_controller.h"
#include "layout_controller.h"

bool Button_attribute_controller::Load(Skin& skin) {
	controller_layout.Set_filename("interfaces/button.yaml");
	controller_layout.Set_skin(&skin);
	if(controller_layout.Load_yaml())
	{
		events[Event(controller_layout.Get_widget("text"), "changed")] = "set_text";
		events[Event(controller_layout.Get_widget("toggle"), "clicked")] = "set_toggle";
		controller_layout.Get_widget("expander")->Set_event_queue(&gui_events);
		return true;
	}
	return false;
}

void Button_attribute_controller::Set_layout_controller(Layout_controller& lc) {
	layout_controller = &lc;
}

Widget* Button_attribute_controller::Get_root() {
	return controller_layout.Get_widget("expander");
}

void Button_attribute_controller::Handle_event(const Ustring& event_handle, const Event& event) {
	Button* button = dynamic_cast<Button*>(layout_controller->Get_current_widget());
	if(button) {
		if(event_handle == "set_text") {
			Inputbox* text = dynamic_cast<Inputbox*>(controller_layout.Get_widget("text"));
			button->Set_text(text->Get_text());
		}
		if(event_handle == "set_toggle") {
			Button* toggle = dynamic_cast<Button*>(controller_layout.Get_widget("toggle"));
			button->Set_toggle(toggle->Is_active());
		}
	}
}

void Button_attribute_controller::Synchronize_values() {
	Button* button = dynamic_cast<Button*>(layout_controller->Get_current_widget());
	Inputbox* text = dynamic_cast<Inputbox*>(controller_layout.Get_widget("text"));
	Button* toggle = dynamic_cast<Button*>(controller_layout.Get_widget("toggle"));
	text->Set_text(button->Get_text());
	toggle->Set_active(button->Is_toggle());
}

void Button_attribute_controller::Update(){
	while(!gui_events.Empty())
	{
		const Event& gui_event = gui_events.Front();
		Process_event(gui_event);
		gui_events.Pop();
	}
}
