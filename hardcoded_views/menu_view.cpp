#include "menu_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <menu.h>
#include <font.h>

Vector2 Menu_view::Request_size(const Widget& widget) const
{
	const Menu& menu = dynamic_cast<const Menu&>(widget);
	int oc = menu.Get_option_count();
	Vector2 size;
	size.y = al_get_font_line_height(font->Afont()) * oc + 6;
	for(int i = 0; i<oc; ++i)
	{
		Ustring o = menu.Get_option(i);
		//int w = al_get_text_width(font->Afont(), o.Cstring());
		int w = font->Get_ustr_width(o.Astring());
		if(w > size.x)
			size.x = w;
	}
	size.x += 6;
	return size;
}

float Menu_view::Get_value(int id, const Widget& widget) const
{
	if(id == Menu::OPTION_HEIGHT)
	{
		return al_get_font_line_height(font->Afont());
	}
	if(id == Menu::PADDING_TOP)
	{
		return 3;
	}
	return 0;
}

void Menu_view::Render(const Widget& widget) const
{
	const Menu& menu = dynamic_cast<const Menu&>(widget);
	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
//	std::cout<<s.y<<std::endl;

	ALLEGRO_COLOR bg_color = al_map_rgb_f(0.5, 0.5, 0.5);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.9, 0.0, 0.0);
	ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR selected_color = al_map_rgb_f(0, 0, 1);

	al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
	al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color, 0);

	int font_h = al_get_font_line_height(font->Afont());
	int y = p.y + 3;
	int x = p.x + 3;

	int selected = menu.Get_selected_option();
	int hover = menu.Get_hover_option();
	int oc = menu.Get_option_count();
	for(int i = 0; i<oc; ++i)
	{
		if(i == selected && !hover==-1) {
			al_draw_filled_rectangle(p.x, y, p.x+s.x-1, y+font_h, selected_color);
		}
		if(i == hover) {
			al_draw_filled_rectangle(p.x, y, p.x+s.x-1, y+font_h, selected_color);
		}
		Ustring o = menu.Get_option(i);
		al_draw_text(font->Afont(), text_color, x, y, 0, o.Cstring());
		y += font_h;
	}
}
