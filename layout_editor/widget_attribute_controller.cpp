#include "widget_attribute_controller.h"
#include "layout_controller.h"

bool Widget_attribute_controller::Load(Skin& skin) {
	layout.Set_filename("interfaces/widget.yaml");
	layout.Set_skin(&skin);
	if(layout.Load_yaml())
	{
		events[Event(layout.Get_widget("name"), "changed")] = "set_name";
		events[Event(layout.Get_widget("fixed width"), "clicked")] = "set_fixed_width";
		events[Event(layout.Get_widget("fixed height"), "clicked")] = "set_fixed_height";
		events[Event(layout.Get_widget("tooltip"), "changed")] = "set_tooltip";
		return true;
	}
	return false;
}

void Widget_attribute_controller::Set_layout_controller(Layout_controller& lc) {
	layout_controller = &lc;
}

Widget* Widget_attribute_controller::Get_root() {
	return layout.Get_widget("expander");
}

void Widget_attribute_controller::Handle_event(const Ustring& event_handle) {
	Widget* widget = layout_controller->Get_current_widget();
	if(widget) {
		if(event_handle == "set_name") {
			Inputbox* name = dynamic_cast<Inputbox*>(layout.Get_widget("name"));

			if(layout_controller->Rename_widget(widget, name->Get_text())) {
				layout_controller->Get_current_tree()->Set_text(name->Get_text());
			} else {
				name->Set_text(widget->Get_name());
			}
		}
		if(event_handle == "set_fixed_width") {
			Button* toggle = dynamic_cast<Button*>(layout.Get_widget("fixed width"));
			widget->Set_fixed_width(toggle->Is_active());
		}
		if(event_handle == "set_fixed_height") {
			Button* toggle = dynamic_cast<Button*>(layout.Get_widget("fixed height"));
			widget->Set_fixed_height(toggle->Is_active());
		}
		if(event_handle == "set_tooltip") {
			Inputbox* tooltip = dynamic_cast<Inputbox*>(layout.Get_widget("tooltip"));
			widget->Set_tooltip(tooltip->Get_text());
		}
	}
}

void Widget_attribute_controller::Synchronize_values() {
	Widget* widget = layout_controller->Get_current_widget();
	Inputbox* name = dynamic_cast<Inputbox*>(layout.Get_widget("name"));
	Button* fixed_width = dynamic_cast<Button*>(layout.Get_widget("fixed width"));
	Button* fixed_height = dynamic_cast<Button*>(layout.Get_widget("fixed height"));
	Inputbox* tooltip = dynamic_cast<Inputbox*>(layout.Get_widget("tooltip"));
	name->Set_text(widget->Get_name());
	fixed_width->Set_active(widget->Has_fixed_width());
	fixed_height->Set_active(widget->Has_fixed_height());
	tooltip->Set_text(widget->Get_tooltip());
}
