#include "button_attribute_controller.h"
#include "layout_controller.h"

bool Button_attribute_controller::Load(Skin& skin) {
	layout.Set_filename("interfaces/button.yaml");
	layout.Set_skin(&skin);
	if(layout.Load_yaml())
	{
		events[Event(layout.Get_widget("text"), "changed")] = "set_text";
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
	if(event_handle == "set_text") {
		std::cout<<"Here we go!"<<std::endl;
		Button* button = dynamic_cast<Button*>(layout_controller->Get_current_widget());
		if(button) {
			std::cout<<"It's a button!"<<std::endl;
			Inputbox* text = dynamic_cast<Inputbox*>(layout.Get_widget("text"));
			button->Set_text(text->Get_text());
		}
	}
}
