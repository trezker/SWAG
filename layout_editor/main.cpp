#include "swag.h"
#include "hardcoded_views.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <stack>

#include "controller.h"
#include "layout_controller.h"
#include "editor_controller.h"
#include "button_attribute_controller.h"
#include "widget_attribute_controller.h"


int main(int argc, char **argv)
{
	al_init();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	
	al_set_new_display_flags(ALLEGRO_WINDOWED|ALLEGRO_RESIZABLE);
	ALLEGRO_DISPLAY *display = al_create_display(640, 480);
	ALLEGRO_DISPLAY *tooldisplay = al_create_display(200, 480);
    al_set_window_position(tooldisplay, 10, 0);
    al_set_window_position(display, 220, 0);
    
	ALLEGRO_DISPLAY *current_display = tooldisplay;


	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)tooldisplay);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	ALLEGRO_TIMER *timer = al_create_timer(0.01);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	ALLEGRO_FONT* font = al_load_font("data/DejaVuSans.ttf", 12, 0);
	if(!font)
		font = al_load_font("examples/data/DejaVuSans.ttf", 12, 0);

	Layout layout;

	Layout_controller layout_controller;
	layout_controller.Set_layout(&layout);

	Tree* widget_tree = layout_controller.Get_skin().Clone<Tree>("tree");
	widget_tree->Set_text("Desktop");
	widget_tree->Select();

	Widget* root_widget = layout_controller.Get_skin().Clone<Desktop>("desktop");
	root_widget->Set_position(Vector2(0, 0));
	root_widget->Set_size(Vector2(640, 480));

	layout_controller.Set_tree(widget_tree, root_widget);
	layout_controller.Set_root(root_widget);
	layout_controller.Set_root_tree(widget_tree);
	layout_controller.Select_tree(widget_tree);

	layout.Set_skin(&layout_controller.Get_skin());
	layout.Add_widget("root", root_widget, NULL);


	Editor_controller editor_controller;
	editor_controller.Set_layout_display(display);
	editor_controller.Set_layout_controller(layout_controller);
	editor_controller.Load(layout_controller.Get_skin());

	Button* removebutton = layout_controller.Get_skin().Clone<Button>("button");
	removebutton->Set_text("Remove");
	removebutton->Set_tooltip("Remove widget and its children");

	Vertical_box* create_vbox = layout_controller.Get_skin().Clone<Vertical_box>("vertical box");

	typedef std::map<Widget*, Ustring> Create_buttons;
	Create_buttons create_buttons;
	Namelist protlist = layout_controller.Get_skin().Get_prototype_list();
	for(Namelist::iterator i = protlist.begin(); i != protlist.end(); ++i)
	{
		Button* createbutton = layout_controller.Get_skin().Clone<Button>("button");
		createbutton->Set_text(*i);
		create_buttons[createbutton] = *i;
		create_vbox->Add(createbutton);
		createbutton->Set_tooltip("Create a widget");
	}

	Expander* create_expander = layout_controller.Get_skin().Clone<Expander>("expander");
	create_expander->Add_child(create_vbox);
	create_expander->Set_text("Create widgets");
	create_expander->Enable_fixed_height();
	create_expander->Open();

	//Widget properties
	Vertical_box* attributes_vbox = layout_controller.Get_skin().Clone<Vertical_box>("vertical box");



	//Widget attribute editing interfaces
	typedef std::map<std::string, Controller*> Attribute_controllers;
	Attribute_controllers attribute_controllers;

	Widget_attribute_controller* widget_controller = new Widget_attribute_controller;
	widget_controller->Load(layout_controller.Get_skin());
	widget_controller->Set_layout_controller(layout_controller);
	attribute_controllers["widget"] = widget_controller;
	attributes_vbox->Add_child(widget_controller->Get_root());

	Button_attribute_controller* button_controller = new Button_attribute_controller;
	button_controller->Load(layout_controller.Get_skin());
	button_controller->Set_layout_controller(layout_controller);
	attribute_controllers["button"] = button_controller;


	//FPS
	Label* fps_label = layout_controller.Get_skin().Clone<Label>("label");
	fps_label->Set_text("FPS: ");

	//Main vbox
	Vertical_box* toolvbox = layout_controller.Get_skin().Clone<Vertical_box>("vertical box");
	toolvbox->Add(editor_controller.Get_root());
//	toolvbox->Add(widget_tree);
	toolvbox->Add(removebutton);
	toolvbox->Add(create_expander);
	toolvbox->Add(attributes_vbox);
	toolvbox->Add(fps_label);

	Slider_box* slider_box = layout_controller.Get_skin().Clone<Slider_box>("slider box");
	slider_box->Set_child(toolvbox);

	Desktop* desktop = layout_controller.Get_skin().Clone<Desktop>("desktop");
	desktop->Set_child(slider_box);
	desktop->Set_position(Vector2(0, 0));
	desktop->Set_size(Vector2(200, 480));

	Widget* toolroot = desktop;

	Event_queue gui_events;
	toolroot->Set_event_queue(&gui_events);

	typedef std::list<double> Frametimes;
	Frametimes frametimes;
	double total_time = 0;
	double last_time = al_current_time();
	bool quit = false;
	while(!quit)
	{
		ALLEGRO_EVENT event;
		while (al_get_next_event(event_queue, &event))
		{
			if (ALLEGRO_EVENT_KEY_DOWN == event.type)
			{
				if (ALLEGRO_KEY_ESCAPE == event.keyboard.keycode)
				{
					quit=true;
				}
			}
			if (ALLEGRO_EVENT_DISPLAY_CLOSE == event.type)
			{
				quit=true;
			}
			if (ALLEGRO_EVENT_DISPLAY_RESIZE == event.type)
			{
				al_acknowledge_resize(event.display.source);
				if(event.display.source == display)
					layout_controller.Get_root()->Set_size(Vector2(event.display.width-20, event.display.height-20));
			}
			if (ALLEGRO_EVENT_DISPLAY_SWITCH_IN == event.type)
			{
				current_display = event.display.source;
			}
			if(current_display == tooldisplay)
			{
				toolroot->Handle_event(event);
			}
			if(current_display == display)
			{
				layout_controller.Get_root()->Handle_event(event);
			}
		}

		while(!gui_events.Empty())
		{
			const Event& gui_event = gui_events.Front();
			for(Attribute_controllers::iterator i = attribute_controllers.begin(); i != attribute_controllers.end(); ++i){
				i->second->Process_event(gui_event);
			}
			editor_controller.Process_event(gui_event);
			std::cout<<"EVENT_PROCESSING: "<<gui_event.type<<std::endl;
			if(gui_event.type == "selected")
			{
				Tree* newsel = dynamic_cast<Tree*>(gui_event.source);
				if(newsel)
				{
					Widget* tw = layout_controller.Get_widget(newsel);
					if(tw)
					{
						layout_controller.Select_tree(newsel);
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
			else if(gui_event.type == "clicked")
			{
				if(layout_controller.Get_current_tree())
				{
					Create_buttons::iterator i = create_buttons.find(gui_event.source);
					if(i != create_buttons.end())
					{
						std::cout<<i->second<<std::endl;
						Widget* child = layout_controller.Get_skin().Clone<Widget>(i->second);
						Container* parent = dynamic_cast<Container*>(layout_controller.Get_current_widget());

						if(parent && parent->Add_child(child))
						{
							Ustring name = layout.Add_widget(i->second, child, parent);
							Tree* tree_child = layout_controller.Get_skin().Clone<Tree>("tree");
							tree_child->Set_text(name);
							layout_controller.Get_current_tree()->Add_child(tree_child);
							layout_controller.Set_tree(tree_child, child);
							layout_controller.Get_current_tree()->Open();
							Text_interface* has_text = dynamic_cast<Text_interface*>(child);
							if(has_text)
								has_text->Set_text(name);

						}
						else
							delete child;
					}
					if(gui_event.source == removebutton)
					{
						Tree* parent = dynamic_cast<Tree*>(layout_controller.Get_current_tree()->Get_parent());
						if(parent)
						{
							dynamic_cast<Container*>(layout_controller.Get_widget(parent))->Remove_child(layout_controller.Get_current_widget());

							parent->Remove_child(layout_controller.Get_current_tree());
							Trees deadlist;
							deadlist.push_back(layout_controller.Get_current_tree());
							layout_controller.Select_tree(NULL);
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
								layout.Remove_widget(layout_controller.Get_widget(current));
								layout_controller.Destroy_widget(current);
								++count;
							}
							layout_controller.Select_tree(parent);
							widget_tree->Select();
						}
					}
				}
			}
			gui_events.Pop();
		}

		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
		
		layout_controller.Get_skin().Update(dt);

		al_set_target_backbuffer(display);
		layout_controller.Get_root()->Render();
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_set_target_backbuffer(tooldisplay);
		toolroot->Render();

		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_rest(0.001);
		
		
		frametimes.push_back(dt);
		total_time+=dt;
		if(frametimes.size()>100)
		{
			total_time -= frametimes.front();
			frametimes.erase(frametimes.begin());
		}
		int fps = frametimes.size()/total_time;
		std::stringstream ss;
		ss<<fps;
		std::string fps_string;
		ss>>fps_string;
		fps_label->Set_text((std::string("FPS: ")+fps_string).c_str());
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_display(tooldisplay);

	return 0;
}
