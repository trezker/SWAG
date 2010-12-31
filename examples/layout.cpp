#include "swag.h"
#include "hardcoded_views.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>

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

	ALLEGRO_FONT* font = al_load_font("data/times.ttf", 12, 0);
	if(!font)
		font = al_load_font("examples/data/times.ttf", 12, 0);

	Widgetmap widgetmap;
	Init_hardcoded_views(widgetmap);
	
	Widget* dyn_child = widgetmap["size mode"]->Clone();
	Widget* fix_child = widgetmap["size mode"]->Clone();
	fix_child->Enable_fixed_height();
	fix_child->Enable_fixed_width();
	Widget* dyn_child2 = widgetmap["size mode"]->Clone();
	Horizontal_paned* hpaned = dynamic_cast<Horizontal_paned*>(widgetmap["horizontal paned"]->Clone());
	hpaned->Set_left(dyn_child2);
	hpaned->Set_pane_fraction(0.5);

	Vertical_box* vbox = dynamic_cast<Vertical_box*>(widgetmap["vertical box"]->Clone());
	vbox->Add(hpaned);
	vbox->Add(fix_child);

	Vertical_paned* root = dynamic_cast<Vertical_paned*>(widgetmap["vertical paned"]->Clone());
	root->Set_pane_fraction(0.5);
	root->Set_position(Vector2(10, 10));
	root->Set_size(Vector2(620, 460));
	root->Set_top(dyn_child);
	root->Set_bottom(vbox);
	root->Organise();

	Expander* widget_tree = dynamic_cast<Expander*>(widgetmap["expander"]->Clone());
	widget_tree->Set_text("Expander");
	widget_tree->Enable_fixed_height();
	widget_tree->Enable_fixed_width();
	
	Button* createbutton = dynamic_cast<Button*>(widgetmap["button"]->Clone());
	createbutton->Set_text("Create");
	createbutton->Enable_fixed_height();

	Button* removebutton = dynamic_cast<Button*>(widgetmap["button"]->Clone());
	removebutton->Set_text("Remove");
	removebutton->Enable_fixed_height();

	Horizontal_box* hbox = dynamic_cast<Horizontal_box*>(widgetmap["horizontal box"]->Clone());
	hbox->Add(createbutton);
	hbox->Add(removebutton);
	hbox->Enable_fixed_height();

	Inputbox* inputbox = dynamic_cast<Inputbox*>(widgetmap["inputbox"]->Clone());
	inputbox->Set_text("Change me");
	inputbox->Enable_fixed_height();

	Vertical_box* toolroot = dynamic_cast<Vertical_box*>(widgetmap["vertical box"]->Clone());
	toolroot->Set_position(Vector2(10, 10));
	toolroot->Set_size(Vector2(180, 460));
	toolroot->Add(widget_tree);
	toolroot->Add(hbox);
	toolroot->Add(inputbox);
	toolroot->Organise();

	Event_queue gui_events;
	toolroot->Set_event_queue(&gui_events);

	Expander* selected_expander = NULL;

//	double last_time = al_current_time();
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
				if(event.display.source == tooldisplay)
					toolroot->Set_size(Vector2(event.display.width-20, event.display.height-20));
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
					if(gui_event.source == createbutton)
					{
						Expander* expand_child = dynamic_cast<Expander*>(widgetmap["expander"]->Clone());
						expand_child->Set_text("Created child");
						selected_expander->Add_child(expand_child);
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
			gui_events.Pop();
		}

/*		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
*/
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
	return 0;
}
