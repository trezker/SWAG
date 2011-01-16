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

int main()
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

	ALLEGRO_DISPLAY *current_display = tooldisplay;


	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)tooldisplay);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	ALLEGRO_TIMER *timer = al_create_timer(0.01);
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	ALLEGRO_FONT* font = al_load_font("data/times.ttf", 12, 0);
	if(!font)
		font = al_load_font("examples/data/times.ttf", 12, 0);

	Hardcoded_skin skin;

	Desktop* root = skin.Clone<Desktop>("desktop");
	root->Set_position(Vector2(0, 0));
	root->Set_size(Vector2(640, 480));


	Tree* widget_tree = skin.Clone<Tree>("tree");
	widget_tree->Set_text("Desktop");
	widget_tree->Enable_fixed_height();
	widget_tree->Enable_fixed_width();
	widget_tree->Select();

	Button* removebutton = skin.Clone<Button>("button");
	removebutton->Set_text("Remove");
	removebutton->Set_tooltip("Remove widget and its children");

	Inputbox* inputbox = skin.Clone<Inputbox>("inputbox");
	inputbox->Set_text("Change me");
	inputbox->Set_tooltip("Change this text");


	Vertical_box* create_vbox = skin.Clone<Vertical_box>("vertical box");

	typedef std::map<Widget*, std::string> Create_buttons;
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

	Label *label_fixed_width = skin.Clone<Label>("label");
	label_fixed_width->Set_text("Fixed_width");

	Expander* widget_properties = skin.Clone<Expander>("expander");
	widget_properties->Add_child(label_fixed_width);
	widget_properties->Set_text("Widget properties");
	widget_properties->Enable_fixed_height();

	Label* fps_label = skin.Clone<Label>("label");
	fps_label->Set_text("FPS: ");

	Vertical_box* toolvbox = skin.Clone<Vertical_box>("vertical box");
	toolvbox->Add(widget_tree);
	toolvbox->Add(removebutton);
	toolvbox->Add(inputbox);
	toolvbox->Add(create_expander);
	toolvbox->Add(widget_properties);
	toolvbox->Add(fps_label);

	Desktop* desktop = skin.Clone<Desktop>("desktop");
	desktop->Set_child(toolvbox);
	desktop->Set_position(Vector2(0, 0));
	desktop->Set_size(Vector2(200, 480));

	Widget* toolroot = desktop;

	Event_queue gui_events;
	toolroot->Set_event_queue(&gui_events);

	Tree* selected_tree = widget_tree;

	typedef std::map<Tree*, Widget*> Treemap;
	Treemap treemap;
	treemap[widget_tree] = root;

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
			if(gui_event.type == "selected")
			{
				Tree* newsel = dynamic_cast<Tree*>(gui_event.source);
				if(newsel)
				{
					selected_tree = newsel;
				}
			}
			else if(gui_event.type == "clicked")
			{
				if(selected_tree)
				{
					Create_buttons::iterator i = create_buttons.find(gui_event.source);
					if(i != create_buttons.end())
					{
						std::cout<<i->second<<std::endl;
						Widget* child = skin.Clone<Widget>(i->second);
						Container* parent = dynamic_cast<Container*>(treemap[selected_tree]);
						if(parent && parent->Add_child(child))
						{
							Tree* tree_child = skin.Clone<Tree>("tree");
							tree_child->Set_text(i->second);
							selected_tree->Add_child(tree_child);
							treemap[tree_child] = child;
							selected_tree->Open();
						}
						else
							delete child;
					}
					if(gui_event.source == removebutton)
					{
						Tree* parent = dynamic_cast<Tree*>(selected_tree->Get_parent());
						if(parent)
						{
							dynamic_cast<Container*>(treemap[parent])->Remove_child(treemap[selected_tree]);

							parent->Remove_child(selected_tree);
							Trees deadlist;
							deadlist.push_back(selected_tree);
							selected_tree = NULL;
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
								delete treemap[current];
								delete current;
								++count;
							}
						}
					}
				}
			}
			else if(gui_event.type == "activated")
			{
				if(selected_tree)
				{
					if(gui_event.source == inputbox)
					{
						selected_tree->Set_text(inputbox->Get_text());
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
		fps_label->Set_text(std::string("FPS: ")+fps_string);
	}

//	delete root;

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_display(tooldisplay);

	return 0;
}
