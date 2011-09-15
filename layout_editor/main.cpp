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

	//FPS
	Label* fps_label = layout_controller.Get_skin().Clone<Label>("label");
	fps_label->Set_text("FPS: ");

	//Main vbox
	Vertical_box* toolvbox = layout_controller.Get_skin().Clone<Vertical_box>("vertical box");
	toolvbox->Add(editor_controller.Get_root());
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
					}
				}
			}
			gui_events.Pop();
		}

		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
		
		editor_controller.Update();
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
