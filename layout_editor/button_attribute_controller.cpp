#include "button_attribute_controller.h"
#include "layout_controller.h"

bool Button_attribute_controller::Load(Skin& skin) {
	layout.Set_filename("interfaces/button.yaml");
	layout.Set_skin(&skin);
	if(layout.Load_yaml())
	{
		events[Event(layout.Get_widget("text"), "changed")] = "set_text";
		events[Event(layout.Get_widget("toggle"), "clicked")] = "set_toggle";
		return true;
	}
	return false;
}

void Button_attribute_controller::Set_layout_controller(Layout_controller& lc) {
	layout_controller = &lc;
}

Widget* Button_attribute_controller::Get_root() {
	return layout.Get_widget("expander");
}

void Button_attribute_controller::Handle_event(const Ustring& event_handle) {
	Button* button = dynamic_cast<Button*>(layout_controller->Get_current_widget());
	if(button) {
		if(event_handle == "set_text") {
			Inputbox* text = dynamic_cast<Inputbox*>(layout.Get_widget("text"));
			button->Set_text(text->Get_text());
		}
		if(event_handle == "set_toggle") {
			Button* toggle = dynamic_cast<Button*>(layout.Get_widget("toggle"));
			button->Set_toggle(toggle->Is_active());
		}
	}
}

void Button_attribute_controller::Synchronize_values() {
	Button* button = dynamic_cast<Button*>(layout_controller->Get_current_widget());
	Inputbox* text = dynamic_cast<Inputbox*>(layout.Get_widget("text"));
	Button* toggle = dynamic_cast<Button*>(layout.Get_widget("toggle"));
	text->Set_text(button->Get_text());
	toggle->Set_active(button->Is_toggle());
}
