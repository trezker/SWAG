#include "swag.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>

class Size_mode_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const
	{
		return Vector2(150, 30);
	}
	
	virtual void Render(const Widget& widget) const
	{
		Vector2 p = widget.Get_position();
		Vector2 s = widget.Get_size();
		bool fw = widget.Has_fixed_width();
		bool fh = widget.Has_fixed_height();
		ALLEGRO_COLOR bg_color = al_map_rgb_f(0.5, 0.5, 0.5);
		ALLEGRO_COLOR edge_color = al_map_rgb_f(0.9, 0.0, 0.0);
		ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);
		al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
		al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color, 0);

		int font_h = al_get_font_line_height(font);
		int y = p.y + (s.y - font_h)/2;
		int x = p.x + s.x/2;
		if(fw && fh)
			al_draw_text(font, text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Fixed");
		else if(fh)
			al_draw_text(font, text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Dynamic width");
		else if(fw)
			al_draw_text(font, text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Dynamic height");
		else
			al_draw_text(font, text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Fully dynamic");
	}
public:
	ALLEGRO_FONT* font;
};

class Box_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const
	{
		Vector2 size;
		const Box& box = dynamic_cast<const Box&>(widget);
		Widgets widgets = box.Get_widgets();
		for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
		{
			Vector2 ws = (*i)->Request_size();
			size.y += ws.y;
			if(ws.x>size.x)
				size.x = ws.x;
		}
		return size;
	}
	
	virtual void Render(const Widget& widget) const
	{
		const Box& box = dynamic_cast<const Box&>(widget);
		Widgets widgets = box.Get_widgets();
		for(Widgets::iterator i = widgets.begin(); i != widgets.end(); ++i)
		{
			(*i)->Render();
		}
	}
};

class Expander_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const
	{
		const Expander& expander = dynamic_cast<const Expander&>(widget);
		Vector2 size;
		const std::string& text = expander.Get_text();
		size.x = al_get_text_width(font, text.c_str()) + 6;
		size.y = al_get_font_line_height(font) + 6;
		float lh = size.y/4+6;

		Expanders children = expander.Get_children();
		size.x += size.y/2;
		if(expander.Is_open())
		{
			for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
			{
				Vector2 ws = (*i)->Request_size();
				size.y += ws.y;
				if(ws.x+lh>size.x)
					size.x = ws.x+lh;
			}
		}
		return size;
	}
	
	virtual void Render(const Widget& widget) const
	{
		const Expander& expander = dynamic_cast<const Expander&>(widget);

		Expanders children = expander.Get_children();
		if(expander.Is_open())
		{
			for(Expanders::iterator i = children.begin(); i != children.end(); ++i)
			{
				(*i)->Render();
			}
		}

		Vector2 p = widget.Get_position();
		Vector2 s = widget.Get_size();
		ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);
		ALLEGRO_COLOR tri_color = al_map_rgb_f(1, 1, 1);
		ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
		ALLEGRO_COLOR select_color = al_map_rgb_f(0.0, 0, 0.8);

		const std::string& text = expander.Get_text();
		float h = al_get_font_line_height(font);

		float text_width = al_get_text_width(font, text.c_str());
		if(expander.Is_selected())
			al_draw_filled_rectangle(p.x+3+h/2, p.y+1, p.x+6+h/2+text_width, p.y+h+6, select_color);

		al_draw_text(font, text_color, p.x+6+h/2, p.y+3, 0, text.c_str());
		if(!children.empty())
		{
			float top = h*0.25;
			float middle = h*0.5;
			float bottom = h*0.75;

			if(expander.Is_open())
			{
				al_draw_filled_triangle(p.x+3, p.y+3+middle, p.x+3+middle, p.y+3+middle, p.x+3+top, p.y+3+bottom, tri_color);
				al_draw_triangle       (p.x+3, p.y+3+middle, p.x+3+middle, p.y+3+middle, p.x+3+top, p.y+3+bottom, edge_color, 0);
			}
			else
			{
				al_draw_filled_triangle(p.x+3, p.y+3+top, p.x+3+top, p.y+3+middle, p.x+3, p.y+3+bottom, tri_color);
				al_draw_triangle       (p.x+3, p.y+3+top, p.x+3+top, p.y+3+middle, p.x+3, p.y+3+bottom, edge_color, 0);
			}
		}
	}
public:
	ALLEGRO_FONT* font;
};

class Vertical_paned_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const
	{
		const Vertical_paned& vertical_paned = dynamic_cast<const Vertical_paned&>(widget);
		Widget* top = vertical_paned.Get_top();
		Widget* bottom = vertical_paned.Get_bottom();

		Vector2 size;
		Widget_view* view;
		if(top)
		{
			Vector2 ws = top->Request_size();
			size.y += ws.y;
			if(ws.x>size.x)
				size.x = ws.x;
		}
		if(bottom)
		{
			Vector2 ws = bottom->Request_size();
			size.y += ws.y;
			if(ws.x>size.x)
				size.x = ws.x;
		}
		size.y+=7;
		return size;
	}
	
	virtual void Render(const Widget& widget) const
	{
		const Vertical_paned& vertical_paned = dynamic_cast<const Vertical_paned&>(widget);
		Widget* top = vertical_paned.Get_top();
		Widget* bottom = vertical_paned.Get_bottom();
		Widget_view* view;
		int clip_x, clip_y, clip_w, clip_h;
		al_get_clipping_rectangle(&clip_x, &clip_y, &clip_w, &clip_h);
		if(top)
		{
			Vector2 p = top->Get_position();
			Vector2 s = top->Get_size();
			al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
			top->Render();
		}
		if(bottom)
		{
			Vector2 p = bottom->Get_position();
			Vector2 s = bottom->Get_size();
			al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
			bottom->Render();
		}
		al_set_clipping_rectangle(clip_x, clip_y, clip_w, clip_h);

		Vector2 p = widget.Get_position();
		Vector2 s = widget.Get_size();
		float pane = p.y+vertical_paned.Get_pane_position()+1;
		ALLEGRO_COLOR bg_color = al_map_rgb_f(0.7, 0.7, 0.7);
		ALLEGRO_COLOR edge_color = al_map_rgb_f(0.3, 0.3, 0.3);
		al_draw_filled_rectangle(p.x, pane, p.x+s.x-1, pane+6, bg_color);
		al_draw_rectangle(p.x, pane, p.x+s.x-1, pane+6, edge_color, 0);
		al_draw_line(p.x+s.x/2-10, pane+2, p.x+s.x/2+10, pane+2, edge_color, 0);
		al_draw_line(p.x+s.x/2-10, pane+4, p.x+s.x/2+10, pane+4, edge_color, 0);
	}
};


class Horizontal_paned_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const
	{
		const Horizontal_paned& horizontal_paned = dynamic_cast<const Horizontal_paned&>(widget);
		Widget* left = horizontal_paned.Get_left();
		Widget* right = horizontal_paned.Get_right();

		Vector2 size;
		Widget_view* view;
		if(left)
		{
			Vector2 ws = left->Request_size();
			size.x += ws.x;
			if(ws.y>size.y)
				size.y = ws.y;
		}
		if(right)
		{
			Vector2 ws = right->Request_size();
			size.x += ws.x;
			if(ws.y>size.y)
				size.y = ws.y;
		}
		size.x+=7;
		return size;
	}
	
	virtual void Render(const Widget& widget) const
	{
		const Horizontal_paned& horizontal_paned = dynamic_cast<const Horizontal_paned&>(widget);
		Widget* left = horizontal_paned.Get_left();
		Widget* right = horizontal_paned.Get_right();
		Widget_view* view;
		int clip_x, clip_y, clip_w, clip_h;
		al_get_clipping_rectangle(&clip_x, &clip_y, &clip_w, &clip_h);
		if(left)
		{
			Vector2 p = left->Get_position();
			Vector2 s = left->Get_size();
			al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
			left->Render();
		}
		if(right)
		{
			Vector2 p = right->Get_position();
			Vector2 s = right->Get_size();
			al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
			right->Render();
		}
		al_set_clipping_rectangle(clip_x, clip_y, clip_w, clip_h);

		Vector2 p = widget.Get_position();
		Vector2 s = widget.Get_size();
		float pane = p.x+horizontal_paned.Get_pane_position();
		ALLEGRO_COLOR bg_color = al_map_rgb_f(0.7, 0.7, 0.7);
		ALLEGRO_COLOR edge_color = al_map_rgb_f(0.3, 0.3, 0.3);
		al_draw_filled_rectangle(pane, p.y+1, pane+6, p.y+s.y, bg_color);
		al_draw_rectangle(pane, p.y+1, pane+6, p.y+s.y, edge_color, 0);
		al_draw_line(pane+2, p.y+s.y/2-10, pane+2, p.y+s.y/2+10, edge_color, 0);
		al_draw_line(pane+4, p.y+s.y/2-10, pane+4, p.y+s.y/2+10, edge_color, 0);
	}
};

class Button_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const
	{
		const Button& button = dynamic_cast<const Button&>(widget);
		const std::string& text = button.Get_text();
		Vector2 size;
		size.x = al_get_text_width(font, text.c_str()) + 6;
		size.y = al_get_font_line_height(font) + 6;
		return size;
	}
	
	virtual void Render(const Widget& widget) const
	{
		const Button& button = dynamic_cast<const Button&>(widget);

		Vector2 p = widget.Get_position();
		Vector2 s = widget.Get_size();
		ALLEGRO_COLOR text_color = al_map_rgb_f(0, 0, 0);
		ALLEGRO_COLOR bg_color = al_map_rgb_f(1, 1, 1);
		ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
		float h = al_get_font_line_height(font);

		if(button.Is_pressed() && button.Is_mouse_over())
		{
			al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color);
			al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color, 0);
		}
		else
		{
			al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
			al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color, 0);
		}

		int font_h = al_get_font_line_height(font);
		int y = p.y + (s.y - font_h)/2;
		int x = p.x + s.x/2;
		const std::string& text = button.Get_text();
		al_draw_text(font, text_color, x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
	}
public:
	ALLEGRO_FONT* font;
};

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


	Size_mode_view size_mode_view;
	size_mode_view.font = font;

	Widget* dyn_child = new Widget();
	dyn_child->Set_view(&size_mode_view);

	Widget* fix_child = new Widget();
	fix_child->Enable_fixed_height();
	fix_child->Enable_fixed_width();
	fix_child->Set_view(&size_mode_view);

	Widget* dyn_child2 = new Widget();
	dyn_child2->Set_view(&size_mode_view);



	Horizontal_paned* hpaned = new Horizontal_paned;
	Horizontal_paned_view hpaned_view;
	hpaned->Set_view(&hpaned_view);
	hpaned->Set_left(dyn_child2);
	hpaned->Set_right(NULL);
	hpaned->Set_pane_fraction(0.5);


	Vertical_box* vbox = new Vertical_box;
	Box_view box_view;
	vbox->Set_view(&box_view);
	vbox->Add(hpaned);
	vbox->Add(fix_child);

	Vertical_paned* root = new Vertical_paned;
	root->Set_pane_fraction(0.5);
	root->Set_position(Vector2(10, 10));
	root->Set_size(Vector2(620, 460));
	Vertical_paned_view root_view;
	root->Set_view(&root_view);
	root->Set_top(dyn_child);
	root->Set_bottom(vbox);
	root->Organise();

	Expander_view expander_view;
	expander_view.font = font;

	Expander* widget_tree = new Expander;
	widget_tree->Set_view(&expander_view);
	widget_tree->Set_text("Expander");
	widget_tree->Enable_fixed_height();
	widget_tree->Enable_fixed_width();

	Button_view button_view;
	button_view.font = font;
	
	Button* createbutton = new Button;
	createbutton->Set_text("Create");
	createbutton->Set_view(&button_view);
	createbutton->Enable_fixed_height();

	Vertical_box* toolroot = new Vertical_box;
	toolroot->Set_position(Vector2(10, 10));
	toolroot->Set_size(Vector2(180, 460));
	toolroot->Set_view(&box_view);
	toolroot->Add(widget_tree);
	toolroot->Add(createbutton);
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
			std::cout<<gui_event.type<<std::endl;
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
				if(gui_event.source == createbutton)
				{
					if(selected_expander)
					{
						Expander* expand_child = new Expander;
						expand_child->Set_view(&expander_view);
						expand_child->Set_text("Created child");
						selected_expander->Add_child(expand_child);
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
