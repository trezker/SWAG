#include "menu_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <menu.h>

Vector2 Menu_view::Request_size(const Widget& widget) const
{
	const Menu& menu = dynamic_cast<const Menu&>(widget);
	int oc = menu.Get_option_count();
	Vector2 size;
	size.y = al_get_font_line_height(font) * oc + 6;
	for(int i = 0; i<oc; ++i)
	{
		std::string o = menu.Get_option(i);
		int w = al_get_text_width(font, o.c_str());
		if(w > size.x)
			size.x = w;
	}
	size.x += 6;
	return size;
}

void Menu_view::Render(const Widget& widget) const
{
	const Menu& menu = dynamic_cast<const Menu&>(widget);
	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();

	ALLEGRO_COLOR bg_color = al_map_rgb_f(0.5, 0.5, 0.5);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.9, 0.0, 0.0);
	ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);

	al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
	al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color, 0);

	int font_h = al_get_font_line_height(font);
	int y = p.y + 3;
	int x = p.x + 3;

	int oc = menu.Get_option_count();
	for(int i = 0; i<oc; ++i)
	{
		std::string o = menu.Get_option(i);
		al_draw_text(font, text_color, x, y, 0, o.c_str());
		y += font_h;
	}
}
