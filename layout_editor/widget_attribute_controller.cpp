#include "widget_attribute_controller.h"
#include "layout_controller.h"

bool Widget_attribute_controller::Load(Skin& skin) {
	controller_layout.Set_filename("interfaces/widget.yaml");
	controller_layout.Set_skin(&skin);
	if(controller_layout.Load_yaml())
	{
		events[Event(controller_layout.Get_widget("name"), "changed")] = "set_name";
		events[Event(controller_layout.Get_widget("fixed width"), "clicked")] = "set_fixed_width";
		events[Event(controller_layout.Get_widget("fixed height"), "clicked")] = "set_fixed_height";
		events[Event(controller_layout.Get_widget("tooltip"), "changed")] = "set_tooltip";
		controller_layout.Get_widget("expander")->Set_event_queue(&gui_events);
		return true;
	}
	return false;
}

void Widget_attribute_controller::Set_layout_controller(Layout_controller& lc) {
	layout_controller = &lc;
}

Widget* Widget_attribute_controller::Get_root() {
	return controller_layout.Get_widget("expander");
}

void Widget_attribute_controller::Handle_event(const Ustring& event_handle, const Event& event) {
	Widget* widget = layout_controller->Get_current_widget();
	if(widget) {
		if(event_handle == "set_name") {
			Inputbox* name = dynamic_cast<Inputbox*>(controller_layout.Get_widget("name"));

			if(layout_controller->Rename_widget(widget, name->Get_text())) {
				layout_controller->Get_current_tree()->Set_text(name->Get_text());
			} else {
				name->Set_text(widget->Get_name());
			}
		}
		if(event_handle == "set_fixed_width") {
			Button* toggle = dynamic_cast<Button*>(controller_layout.Get_widget("fixed width"));
			widget->Set_fixed_width(toggle->Is_active());
		}
		if(event_handle == "set_fixed_height") {
			Button* toggle = dynamic_cast<Button*>(controller_layout.Get_widget("fixed height"));
			widget->Set_fixed_height(toggle->Is_active());
		}
		if(event_handle == "set_tooltip") {
			Inputbox* tooltip = dynamic_cast<Inputbox*>(controller_layout.Get_widget("tooltip"));
			widget->Set_tooltip(tooltip->Get_text());
		}
	}
}

void Widget_attribute_controller::Synchronize_values() {
	Widget* widget = layout_controller->Get_current_widget();
	Inputbox* name = dynamic_cast<Inputbox*>(controller_layout.Get_widget("name"));
	Button* fixed_width = dynamic_cast<Button*>(controller_layout.Get_widget("fixed width"));
	Button* fixed_height = dynamic_cast<Button*>(controller_layout.Get_widget("fixed height"));
	Inputbox* tooltip = dynamic_cast<Inputbox*>(controller_layout.Get_widget("tooltip"));
	name->Set_text(widget->Get_name());
	fixed_width->Set_active(widget->Has_fixed_width());
	fixed_height->Set_active(widget->Has_fixed_height());
	tooltip->Set_text(widget->Get_tooltip());
}

void Widget_attribute_controller::Update(){
	while(!gui_events.Empty())
	{
		const Event& gui_event = gui_events.Front();
		Process_event(gui_event);
		gui_events.Pop();
	}
}
