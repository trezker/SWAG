#include "horizontal_slider_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <horizontal_slider.h>

Vector2 Horizontal_slider_view::Request_size(const Widget& widget) const
{
	const Horizontal_slider& horizontal_slider = dynamic_cast<const Horizontal_slider&>(widget);

	Vector2 size(horizontal_slider.Get_pane_size(), 20);
	return size;
}

void Horizontal_slider_view::Render(const Widget& widget) const
{
	const Horizontal_slider& horizontal_slider = dynamic_cast<const Horizontal_slider&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	float pane = p.x+horizontal_slider.Get_pane_position();
	int pane_size = horizontal_slider.Get_pane_size();

	ALLEGRO_COLOR bg_color = al_map_rgb_f(0.7, 0.7, 0.7);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.3, 0.3, 0.3);

	al_draw_filled_rectangle(pane, p.y+1, pane+pane_size-1, p.y+s.y, bg_color);
	al_draw_rectangle(pane, p.y+1, pane+pane_size-1, p.y+s.y, edge_color, 0);
}
