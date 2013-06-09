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

#include "layout_controller.h"
#include "editor_controller.h"

const char* get_event_name(int type)
{
	switch(type){
		case ALLEGRO_EVENT_KEY_DOWN:
			return "ALLEGRO_EVENT_KEY_DOWN";
			break;
		case ALLEGRO_EVENT_KEY_UP:
			return "ALLEGRO_EVENT_KEY_UP";
			break;
		case ALLEGRO_EVENT_KEY_CHAR:
			return "ALLEGRO_EVENT_KEY_CHAR";
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			return "ALLEGRO_EVENT_MOUSE_AXES";
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			return "ALLEGRO_EVENT_MOUSE_BUTTON_DOWN";
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			return "ALLEGRO_EVENT_MOUSE_BUTTON_UP";
			break;
		case ALLEGRO_EVENT_MOUSE_WARPED:
			return "ALLEGRO_EVENT_MOUSE_WARPED";
			break;
		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
			return "ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY";
			break;
		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			return "ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY";
			break;
		case ALLEGRO_EVENT_TIMER:
			return "ALLEGRO_EVENT_TIMER";
			break;
		case ALLEGRO_EVENT_DISPLAY_EXPOSE:
			return "ALLEGRO_EVENT_DISPLAY_EXPOSE";
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			return "ALLEGRO_EVENT_DISPLAY_RESIZE";
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			return "ALLEGRO_EVENT_DISPLAY_CLOSE";
			break;
		case ALLEGRO_EVENT_DISPLAY_LOST:
			return "ALLEGRO_EVENT_DISPLAY_LOST";
			break;
		case ALLEGRO_EVENT_DISPLAY_FOUND:
			return "ALLEGRO_EVENT_DISPLAY_FOUND";
			break;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
			return "ALLEGRO_EVENT_DISPLAY_SWITCH_OUT";
			break;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
			return "ALLEGRO_EVENT_DISPLAY_SWITCH_IN";
			break;
		case ALLEGRO_EVENT_DISPLAY_ORIENTATION:
			return "ALLEGRO_EVENT_DISPLAY_ORIENTATION";
			break;
	}
}

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
	ALLEGRO_TIMER* timer = al_create_timer(1);

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)tooldisplay);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

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
	fps_label->Set_text("FPS: 100");

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

	al_start_timer(timer);
	bool quit = false;
	while (!quit)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);
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
		
		if(ALLEGRO_EVENT_TIMER == event.type) {
			double dt = al_get_timer_speed(timer);
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

			double fps = 1 / dt;
			std::stringstream ss;
			ss<<fps;
			std::string fps_string;
			ss>>fps_string;
			fps_label->Set_text((std::string("FPS: ")+fps_string).c_str());

			if(dt < 1 && event.timer.count < al_get_timer_count(timer) - 4)
			{
				dt = dt * 1.1;
				al_set_timer_speed(timer, dt);
				//print("Tick too fast, setting speed to " .. timer_speed);
			}
			if(event.timer.count == al_get_timer_count(timer))
			{
				dt = dt * 0.9;
				al_set_timer_speed(timer, dt);
				//print("Tick too fast, setting speed to " .. timer_speed);
			}

			al_rest(0.001);
		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_display(tooldisplay);

	return 0;
}
