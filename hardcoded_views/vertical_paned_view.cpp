#include "vertical_paned_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <vertical_paned.h>

Vector2 Vertical_paned_view::Request_size(const Widget& widget) const
{
	const Vertical_paned& vertical_paned = dynamic_cast<const Vertical_paned&>(widget);
	Widget* top = vertical_paned.Get_top();
	Widget* bottom = vertical_paned.Get_bottom();

	Vector2 size;
	Widget_view* view;
	if(top)
	{
		Vector2 ws = top->Request_size();
		size.y += ws.y;
		if(ws.x>size.x)
			size.x = ws.x;
	}
	if(bottom)
	{
		Vector2 ws = bottom->Request_size();
		size.y += ws.y;
		if(ws.x>size.x)
			size.x = ws.x;
	}
	size.y+=7;
	return size;
}

float Vertical_paned_view::Get_value(int id, const Widget& widget) const
{
	return 7;
}

void Vertical_paned_view::Render(const Widget& widget) const
{
	const Vertical_paned& vertical_paned = dynamic_cast<const Vertical_paned&>(widget);
	Widget* top = vertical_paned.Get_top();
	Widget* bottom = vertical_paned.Get_bottom();
	Widget_view* view;
	int clip_x, clip_y, clip_w, clip_h;
	al_get_clipping_rectangle(&clip_x, &clip_y, &clip_w, &clip_h);
	if(top)
	{
		Vector2 p = top->Get_position();
		Vector2 s = top->Get_size();
		al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
		top->Render();
	}
	if(bottom)
	{
		Vector2 p = bottom->Get_position();
		Vector2 s = bottom->Get_size();
		al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
		bottom->Render();
	}
	al_set_clipping_rectangle(clip_x, clip_y, clip_w, clip_h);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	float pane = p.y+vertical_paned.Get_pane_position()+1;
	ALLEGRO_COLOR bg_color = al_map_rgb_f(0.7, 0.7, 0.7);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.3, 0.3, 0.3);
	al_draw_filled_rectangle(p.x, pane, p.x+s.x-1, pane+6, bg_color);
	al_draw_rectangle(p.x, pane, p.x+s.x-1, pane+6, edge_color, 0);
	al_draw_line(p.x+s.x/2-10, pane+2, p.x+s.x/2+10, pane+2, edge_color, 0);
	al_draw_line(p.x+s.x/2-10, pane+4, p.x+s.x/2+10, pane+4, edge_color, 0);
}
