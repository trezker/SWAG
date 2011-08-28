#include "swag.h"
#include "hardcoded_views.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <stack>

#include "controller.h"
#include "layout_controller.h"
#include "button_attribute_controller.h"


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

	Hardcoded_skin skin;

	Layout layout;

	Widget* root = skin.Clone<Desktop>("desktop");
	root->Set_position(Vector2(0, 0));
	root->Set_size(Vector2(640, 480));
	
	layout.Add_widget("root", root, NULL);

	Button* save_button = skin.Clone<Button>("button");
	save_button->Set_text("Save");

	Button* load_button = skin.Clone<Button>("button");
	load_button->Set_text("Load");
	
	Horizontal_box* file_box = skin.Clone<Horizontal_box>("horizontal box");
	file_box->Add_child(save_button);
	file_box->Add_child(load_button);

	Tree* widget_tree = skin.Clone<Tree>("tree");
	widget_tree->Set_text("Desktop");
	widget_tree->Select();

	Button* removebutton = skin.Clone<Button>("button");
	removebutton->Set_text("Remove");
	removebutton->Set_tooltip("Remove widget and its children");

	Inputbox* inputbox = skin.Clone<Inputbox>("inputbox");
	inputbox->Set_text("Change me");
	inputbox->Set_tooltip("Change this text");


	Vertical_box* create_vbox = skin.Clone<Vertical_box>("vertical box");

	typedef std::map<Widget*, Ustring> Create_buttons;
	Create_buttons create_buttons;
	Namelist protlist = skin.Get_prototype_list();
	for(Namelist::iterator i = protlist.begin(); i != protlist.end(); ++i)
	{
		Button* createbutton = skin.Clone<Button>("button");
		createbutton->Set_text(*i);
		create_buttons[createbutton] = *i;
		create_vbox->Add(createbutton);
		createbutton->Set_tooltip("Create a widget");
	}

	Expander* create_expander = skin.Clone<Expander>("expander");
	create_expander->Add_child(create_vbox);
	create_expander->Set_text("Create widgets");
	create_expander->Enable_fixed_height();
	create_expander->Open();

	//Widget properties

	Label *name_label = skin.Clone<Label>("label");
	name_label->Set_text("Name: ");
	Inputbox *name = skin.Clone<Inputbox>("inputbox");

	Button *fixed_width = skin.Clone<Button>("checkbox");
	fixed_width->Set_text("Fixed width");

	Button *fixed_height = skin.Clone<Button>("checkbox");
	fixed_height->Set_text("Fixed height");

	Label *tooltip_label = skin.Clone<Label>("label");
	tooltip_label->Set_text("Tooltip: ");
	Inputbox *tooltip = skin.Clone<Inputbox>("inputbox");
	
	Horizontal_box* name_hbox = skin.Clone<Horizontal_box>("horizontal box");
	name_hbox->Add_child(name_label);
	name_hbox->Add_child(name);

	Horizontal_box* tooltip_hbox = skin.Clone<Horizontal_box>("horizontal box");
	tooltip_hbox->Add_child(tooltip_label);
	tooltip_hbox->Add_child(tooltip);

	Vertical_box* widget_properties_vbox = skin.Clone<Vertical_box>("vertical box");
	widget_properties_vbox->Add_child(name_hbox);
	widget_properties_vbox->Add_child(fixed_width);
	widget_properties_vbox->Add_child(fixed_height);
	widget_properties_vbox->Add_child(tooltip_hbox);

	Expander* widget_properties = skin.Clone<Expander>("expander");
	widget_properties->Add_child(widget_properties_vbox);
	widget_properties->Set_text("Widget properties");
	widget_properties->Enable_fixed_height();

	Vertical_box* attributes_vbox = skin.Clone<Vertical_box>("vertical box");
	attributes_vbox->Add_child(widget_properties);



	Layout_controller layout_controller;
	layout_controller.Set_tree(widget_tree, root);
	layout_controller.Set_root_tree(widget_tree);
	layout_controller.Select_tree(widget_tree);

	//Widget attribute editing interfaces
	typedef std::map<std::string, Controller*> Attribute_controllers;
	Attribute_controllers attribute_controllers;

	Button_attribute_controller* controller = new Button_attribute_controller;
	controller->Load(skin);
	controller->Set_layout_controller(layout_controller);
	attribute_controllers["button"] = controller;


	//FPS
	Label* fps_label = skin.Clone<Label>("label");
	fps_label->Set_text("FPS: ");

	//Main vbox
	Vertical_box* toolvbox = skin.Clone<Vertical_box>("vertical box");
	toolvbox->Add(file_box);
	toolvbox->Add(widget_tree);
	toolvbox->Add(removebutton);
	toolvbox->Add(inputbox);
	toolvbox->Add(create_expander);
	toolvbox->Add(attributes_vbox);
	toolvbox->Add(fps_label);

	Slider_box* slider_box = skin.Clone<Slider_box>("slider box");
	slider_box->Set_child(toolvbox);

	Desktop* desktop = skin.Clone<Desktop>("desktop");
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
					root->Set_size(Vector2(event.display.width-20, event.display.height-20));
//				if(event.display.source == tooldisplay)
//					toolroot->Set_size(Vector2(event.display.width-20, event.display.height-20));
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
				root->Handle_event(event);
			}
		}

		while(!gui_events.Empty())
		{
			const Event& gui_event = gui_events.Front();
//			std::cout<<gui_event.type<<std::endl;
			for(Attribute_controllers::iterator i = attribute_controllers.begin(); i != attribute_controllers.end(); ++i){
				i->second->Handle_event(gui_event);
			}
			if(gui_event.type == "selected")
			{
				Tree* newsel = dynamic_cast<Tree*>(gui_event.source);
				if(newsel)
				{
					Widget* tw = layout_controller.Get_widget(newsel);
					if(tw)
					{
						layout_controller.Select_tree(newsel);
						if(tw->Has_fixed_width())
							fixed_width->Activate();
						else
							fixed_width->Deactivate();
						if(tw->Has_fixed_height())
							fixed_height->Activate();
						else
							fixed_height->Deactivate();
						name->Set_text(tw->Get_name());
						tooltip->Set_text(tw->Get_tooltip());
						Widgets widgets = attributes_vbox->Get_children();
						for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i) {
							attributes_vbox->Remove_child(*i);
						}
						attributes_vbox->Add_child(widget_properties);
/*						Attribute_interfaces::iterator ai = attribute_interfaces.find(tw->Get_name().Cstring());
						if(ai != attribute_interfaces.end()) {
							attributes_vbox->Add_child(ai->second);
						}
*/						Attribute_controllers::iterator ac = attribute_controllers.find(tw->Get_name().Cstring());
						if(ac != attribute_controllers.end()) {
							attributes_vbox->Add_child(ac->second->Get_root());
						}
					}
				}
			}
			else if(gui_event.type == "changed")
			{
				if(gui_event.source == tooltip)
				{
					Widget* tw = layout_controller.Get_current_widget();
					tw->Set_tooltip(tooltip->Get_text().Cstring());
				}
				if(gui_event.source == name)
				{
					//Must not be empty string, must not collide in layout.
					Widget* tw = layout_controller.Get_current_widget();
					if(layout.Rename_widget(tw, name->Get_text())) {
						layout_controller.Get_current_tree()->Set_text(name->Get_text());
					} else {
						name->Set_text(tw->Get_name());
					}
				}
			}
			else if(gui_event.type == "clicked")
			{
				if(gui_event.source == save_button)
				{
					layout.Set_filename("testlayout.yaml");
					bool s = layout.Save_yaml();
					std::cout<<(s?"Saved":"Save failed")<<std::endl;
				}
				if(gui_event.source == load_button)
				{
					layout_controller.Clear();
					layout.Set_filename("testlayout.yaml");
//					layout.Set_filename("testlayout.xml");
					layout.Set_skin(&skin);
//					bool s = layout.Load();
					if(layout.Load_yaml())
					{
						std::cout<<"Loaded"<<std::endl;
						const Name_to_widget& layout_widgets = layout.Get_widgets();

						root = layout.Get_root();
						root->Set_size(Vector2(al_get_display_width(display), al_get_display_height(display)));
						layout_controller.Set_tree(widget_tree, root);

						typedef std::stack<Tree*> Trees_todo;
						Trees_todo trees_todo;
						trees_todo.push(widget_tree);
						
						while(!trees_todo.empty())
						{
							Tree* current_tree = trees_todo.top();
							trees_todo.pop();

							Container* parent = dynamic_cast<Container*>(layout_controller.Get_widget(current_tree));

							if(parent)
							{
								Widgets children = parent->Get_children();
								for(Widgets::iterator i = children.begin(); i != children.end(); ++i)
								{
									Tree* tree_child = skin.Clone<Tree>("tree");
									tree_child->Set_text((*i)->Get_name());
									layout_controller.Set_tree(tree_child, *i);
									current_tree->Add_child(tree_child);
									trees_todo.push(tree_child);
								}
							}
						}
					}
					else
					{
						root = skin.Clone<Desktop>("desktop");
						root->Set_position(Vector2(0, 0));
						root->Set_size(Vector2(al_get_display_width(display), al_get_display_height(display)));
						
						layout.Add_widget("root", root, NULL);
						layout_controller.Set_tree(widget_tree, root);
					}
					widget_tree->Select();
					layout_controller.Select_tree(widget_tree);
				}
				if(gui_event.source == fixed_width)
				{
					Widget* tw = layout_controller.Get_current_widget();
					if(fixed_width->Is_active())
						tw->Enable_fixed_width();
					else
						tw->Disable_fixed_width();
				}
				if(gui_event.source == fixed_height)
				{
					Widget* tw = layout_controller.Get_current_widget();
					if(fixed_height->Is_active())
						tw->Enable_fixed_height();
					else
						tw->Disable_fixed_height();
				}
				if(layout_controller.Get_current_tree())
				{
					Create_buttons::iterator i = create_buttons.find(gui_event.source);
					if(i != create_buttons.end())
					{
						std::cout<<i->second<<std::endl;
						Widget* child = skin.Clone<Widget>(i->second);
						Container* parent = dynamic_cast<Container*>(layout_controller.Get_current_widget());

						if(parent && parent->Add_child(child))
						{
							Ustring name = layout.Add_widget(i->second, child, parent);
							Tree* tree_child = skin.Clone<Tree>("tree");
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
			else if(gui_event.type == "activated")
			{
				if(layout_controller.Get_current_tree())
				{
					if(gui_event.source == inputbox)
					{
						layout_controller.Get_current_tree()->Set_text(inputbox->Get_text());
					}
				}
			}
			gui_events.Pop();
		}

		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
		
		skin.Update(dt);

		al_set_target_backbuffer(display);
		root->Render();
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

//	delete root;

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_display(tooldisplay);

	return 0;
}
