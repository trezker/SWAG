#include "swag.h"
#include "hardcoded_views.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>
#include <string>
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
/*	
	Widget* dyn_child = skin.Clone<Widget>("size mode");
	Widget* fix_child = skin.Clone<Widget>("size mode");
	fix_child->Enable_fixed_height();
	fix_child->Enable_fixed_width();
	Widget* dyn_child2 = skin.Clone<Widget>("size mode");
	Horizontal_paned* hpaned = skin.Clone<Horizontal_paned>("horizontal paned");
	hpaned->Set_left(dyn_child2);
	hpaned->Set_pane_fraction(0.5);

	Vertical_box* vbox = skin.Clone<Vertical_box>("vertical box");
	vbox->Add(hpaned);
	vbox->Add(fix_child);

	Vertical_paned* root = skin.Clone<Vertical_paned>("vertical paned");
	root->Set_pane_fraction(0.5);
	root->Set_position(Vector2(10, 10));
	root->Set_size(Vector2(620, 460));
	root->Set_top(dyn_child);
	root->Set_bottom(vbox);
	root->Organise();
*/
	Desktop* root = skin.Clone<Desktop>("desktop");
	root->Set_position(Vector2(0, 0));
	root->Set_size(Vector2(640, 480));


	Expander* widget_tree = skin.Clone<Expander>("expander");
	widget_tree->Set_text("Expander");
	widget_tree->Enable_fixed_height();
	widget_tree->Enable_fixed_width();
	
	Button* createbutton = skin.Clone<Button>("button");
	createbutton->Set_text("Create");
	createbutton->Enable_fixed_height();

	Button* removebutton = skin.Clone<Button>("button");
	removebutton->Set_text("Remove");
	removebutton->Enable_fixed_height();
	removebutton->Set_tooltip("Remove widget and its children");

	Horizontal_box* hbox = skin.Clone<Horizontal_box>("horizontal box");
	hbox->Add(createbutton);
	hbox->Add(removebutton);
	hbox->Enable_fixed_height();

	Inputbox* inputbox = skin.Clone<Inputbox>("inputbox");
	inputbox->Set_text("Change me");
	inputbox->Enable_fixed_height();
	inputbox->Set_tooltip("Change this text");


	Vertical_box* toolvbox = skin.Clone<Vertical_box>("vertical box");
	toolvbox->Add(widget_tree);
	toolvbox->Add(hbox);
	toolvbox->Add(inputbox);

	typedef std::map<Widget*, std::string> Create_buttons;
	Create_buttons create_buttons;
	Namelist protlist = skin.Get_prototype_list();
	for(Namelist::iterator i = protlist.begin(); i != protlist.end(); ++i)
	{
		Button* createbutton = skin.Clone<Button>("button");
		createbutton->Set_text(*i);
		createbutton->Enable_fixed_height();
		create_buttons[createbutton] = *i;
		toolvbox->Add(createbutton);
		createbutton->Set_tooltip("Create a widget");
	}

	Desktop* desktop = skin.Clone<Desktop>("desktop");
	desktop->Set_child(toolvbox);
	desktop->Set_position(Vector2(0, 0));
	desktop->Set_size(Vector2(200, 480));

	Widget* toolroot = desktop;

	Event_queue gui_events;
	toolroot->Set_event_queue(&gui_events);

	Expander* selected_expander = NULL;

	typedef std::map<Expander*, Widget*> Treemap;
	Treemap treemap;
	treemap[widget_tree] = root;

	double last_time = al_current_time();
	bool quit = false;
	while(!quit)
	{
		ALLEGRO_EVENT event;
		if (al_get_next_event(event_queue, &event))
		{
			if (ALLEGRO_EVENT_KEY_DOWN == event.type)
			{
				if (ALLEGRO_KEY_ESCAPE == event.keyboard.keycode)
				{
					break;
				}
			}
			if (ALLEGRO_EVENT_DISPLAY_CLOSE == event.type)
			{
				break;
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
				Expander* newsel = dynamic_cast<Expander*>(gui_event.source);
				if(newsel)
				{
					selected_expander = newsel;
				}
			}
			else if(gui_event.type == "clicked")
			{
				if(selected_expander)
				{
					Create_buttons::iterator i = create_buttons.find(gui_event.source);
					if(i != create_buttons.end())
					{
						std::cout<<i->second<<std::endl;
						Widget* child = skin.Clone<Widget>(i->second);
						Container* parent = dynamic_cast<Container*>(treemap[selected_expander]);
						if(parent && parent->Add_child(child))
						{
							Expander* tree_child = skin.Clone<Expander>("expander");
							tree_child->Set_text(i->second);
							selected_expander->Add_child(tree_child);
							treemap[tree_child] = child;
						}
						else
							delete child;
					}
					if(gui_event.source == createbutton)
					{
						Vertical_paned* child = skin.Clone<Vertical_paned>("vertical paned");
						Container* parent = dynamic_cast<Container*>(treemap[selected_expander]);
						if(parent->Add_child(child))
						{
							Expander* tree_child = skin.Clone<Expander>("expander");
							tree_child->Set_text("VPaned");
							selected_expander->Add_child(tree_child);
							treemap[tree_child] = child;
						}
						else
							delete child;
					}
					if(gui_event.source == removebutton)
					{
						Expander* parent = dynamic_cast<Expander*>(selected_expander->Get_parent());
						if(parent)
						{
							parent->Remove_child(selected_expander);
							Expanders deadlist;
							deadlist.push_back(selected_expander);
							selected_expander = NULL;
							int count = 0;
							while(!deadlist.empty())
							{
								Expander* current = deadlist.back();
								deadlist.erase(--deadlist.end());
								Expanders& children = current->Get_children();
								for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
								{
									deadlist.push_back(*i);
								}
								delete current;
								++count;
							}
						}
					}
				}
			}
			else if(gui_event.type == "activated")
			{
				if(selected_expander)
				{
					if(gui_event.source == inputbox)
					{
						selected_expander->Set_text(inputbox->Get_text());
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
	}

//	delete root;

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_display(tooldisplay);

	return 0;
}
