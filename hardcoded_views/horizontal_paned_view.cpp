#include "horizontal_paned_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <horizontal_paned.h>

Vector2 Horizontal_paned_view::Request_size(const Widget& widget) const
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

float Horizontal_paned_view::Get_value(int id, const Widget& widget) const
{
	return 7;
}

void Horizontal_paned_view::Render(const Widget& widget) const
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
