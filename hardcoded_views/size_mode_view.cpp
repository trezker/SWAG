#include "size_mode_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <widget.h>
#include <font.h>

Vector2 Size_mode_view::Request_size(const Widget& widget) const
{
	return Vector2(150, 30);
}

void Size_mode_view::Render(const Widget& widget) const
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

	int font_h = al_get_font_line_height(font->Afont());
	int y = p.y + (s.y - font_h)/2;
	int x = p.x + s.x/2;
	if(fw && fh)
		al_draw_text(font->Afont(), text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Fixed");
	else if(fh)
		al_draw_text(font->Afont(), text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Dynamic width");
	else if(fw)
		al_draw_text(font->Afont(), text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Dynamic height");
	else
		al_draw_text(font->Afont(), text_color, x, y, ALLEGRO_ALIGN_CENTRE, "Fully dynamic");
}
