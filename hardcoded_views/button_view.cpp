#include "button_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <button.h>
#include <font.h>

Vector2 Button_view::Request_size(const Widget& widget) const
{
	const Button& button = dynamic_cast<const Button&>(widget);
	const Ustring& text = button.Get_text();
	Vector2 size;
	//int minwidth = 50;
	//size.x = 50;
	int minwidth = font->Get_ustr_width("Button") + 6;
	size.x = font->Get_ustr_width(text.Astring()) + 6;
	if(size.x<minwidth)
		size.x = minwidth;
	size.y = al_get_font_line_height(font->Afont()) + 6;
	return size;
}

void Button_view::Render(const Widget& widget) const
{
	const Button& button = dynamic_cast<const Button&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(0, 0, 0);
	ALLEGRO_COLOR bg_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
	float h = al_get_font_line_height(font->Afont());

	if((button.Is_pressed() && button.Is_mouse_over()) || (button.Is_toggle() && button.Is_active()))
	{
		al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color);
		al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color, 0);
	}
	else
	{
		al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
		al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color, 0);
	}

	int font_h = al_get_font_line_height(font->Afont());
	int y = p.y + (s.y - font_h)/2;
	int x = p.x + s.x/2;
	const Ustring& text = button.Get_text();
	al_draw_ustr(font->Afont(), text_color, x, y, ALLEGRO_ALIGN_CENTRE, text.Astring());
}
