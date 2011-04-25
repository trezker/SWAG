#include "checkbox_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <button.h>

Vector2 Checkbox_view::Request_size(const Widget& widget) const
{
	const Button& checkbox = dynamic_cast<const Button&>(widget);
	const Ustring& text = checkbox.Get_text();
	Vector2 size;
	int font_h = al_get_font_line_height(font);
	size.y = font_h + 6;
	size.x = size.y + al_get_ustr_width(font, text.Astring()) + 6;
	return size;
}

void Checkbox_view::Render(const Widget& widget) const
{
	const Button& checkbox = dynamic_cast<const Button&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(0, 0, 0);
	ALLEGRO_COLOR bg_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
	float h = al_get_font_line_height(font);

	al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
	al_draw_rectangle(p.x+1, p.y+3, p.x+s.y-3, p.y+s.y-1, edge_color, 0);
	if(checkbox.Is_active())
	{
		al_draw_line(p.x+3, p.y+5, p.x+s.y-4, p.y+s.y-4, edge_color, h/5);
		al_draw_line(p.x+3, p.y+s.y-4, p.x+s.y-4, p.y+5, edge_color, h/5);
	}

	int font_h = al_get_font_line_height(font);
	int y = p.y + 3;
	int x = p.x + s.y + 3;
	const Ustring& text = checkbox.Get_text();
	al_draw_ustr(font, text_color, x, y, 0, text.Astring());
}
