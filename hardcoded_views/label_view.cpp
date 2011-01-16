#include "label_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <label.h>

Vector2 Label_view::Request_size(const Widget& widget) const
{
	const Label& label = dynamic_cast<const Label&>(widget);
	std::string text = label.Get_text();
	Vector2 size;
	size.x = al_get_text_width(font, text.c_str()) + 6;
	size.y = al_get_font_line_height(font) + 6;
	return size;
}

void Label_view::Render(const Widget& widget) const
{
	const Label& label = dynamic_cast<const Label&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(0, 0, 0);
	ALLEGRO_COLOR bg_color = al_map_rgb_f(1, 1, 1);

	al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);

	int y = p.y + 3;
	int x = p.x + 3;
	std::string text = label.Get_text();
	al_draw_text(font, text_color, x, y, 0, text.c_str());
}
