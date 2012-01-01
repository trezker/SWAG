#include "dropdown_menu_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <dropdown_menu.h>
#include <menu.h>

Vector2 Dropdown_menu_view::Request_size(const Widget& widget) const
{
	const Dropdown_menu& dropdown_menu = dynamic_cast<const Dropdown_menu&>(widget);
	Vector2 size;
	//const Ustring& text = dropdown_menu.Get_text();
	Menu *child = dynamic_cast<Menu*>(dropdown_menu.Get_child());
	if(child){
		const Ustring& text = child->Get_option(child->Get_selected_option());
		size.x = al_get_ustr_width(font, text.Astring()) + 6;
	}
	size.y = al_get_font_line_height(font) + 6;
	float lh = size.y/4+6;

	size.x += size.y/2;
	return size;
}

float Dropdown_menu_view::Get_value(int id, const Widget& widget) const
{
	if(id == Dropdown_menu::SELF_HEIGHT)
	{
		return al_get_font_line_height(font) + 6;
	}
	return 0;
}

void Dropdown_menu_view::Render(const Widget& widget) const
{
	const Dropdown_menu& dropdown_menu = dynamic_cast<const Dropdown_menu&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR tri_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
	ALLEGRO_COLOR select_color = al_map_rgb_f(0.0, 0, 0.8);

	//const Ustring& text = dropdown_menu.Get_text();
	float h = al_get_font_line_height(font);

	Menu *child = dynamic_cast<Menu*>(dropdown_menu.Get_child());
	if(child){
		const Ustring& text = child->Get_option(child->Get_selected_option());
		float text_width = al_get_ustr_width(font, text.Astring());
		al_draw_ustr(font, text_color, p.x+6+h/2, p.y+3, 0, text.Astring());
	}


	float top = h*0.25;
	float middle = h*0.5;
	float bottom = h*0.75;

	if(dropdown_menu.Is_open())
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
