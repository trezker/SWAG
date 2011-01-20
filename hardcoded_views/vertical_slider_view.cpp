#include "vertical_slider_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <vertical_slider.h>

Vector2 Vertical_slider_view::Request_size(const Widget& widget) const
{
	const Vertical_slider& vertical_slider = dynamic_cast<const Vertical_slider&>(widget);

	Vector2 size(20, vertical_slider.Get_pane_size());
	return size;
}

void Vertical_slider_view::Render(const Widget& widget) const
{
	const Vertical_slider& vertical_slider = dynamic_cast<const Vertical_slider&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	float pane = p.y+vertical_slider.Get_pane_position()+1;
	int pane_size = vertical_slider.Get_pane_size();
	ALLEGRO_COLOR bg_color = al_map_rgb_f(0.7, 0.7, 0.7);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.3, 0.3, 0.3);
	al_draw_filled_rectangle(p.x, pane, p.x+s.x-1, pane+pane_size-1, bg_color);
	al_draw_rectangle(p.x, pane, p.x+s.x-1, pane+pane_size-1, edge_color, 0);
}
