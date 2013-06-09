#include "expander_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <expander.h>
#include "font.h"

Vector2 Expander_view::Request_size(const Widget& widget) const
{
	const Expander& expander = dynamic_cast<const Expander&>(widget);
	Vector2 size;
	const Ustring& text = expander.Get_text();
	size.x = al_get_ustr_width(font->Afont(), text.Astring()) + 6;
	size.y = al_get_font_line_height(font->Afont()) + 6;
	float lh = size.y/4+6;

	Widget* child = expander.Get_child();
	size.x += size.y/2;
	if(child && expander.Is_open())
	{
		Vector2 ws = child->Request_size();
		size.y += ws.y;
		if(ws.x+lh>size.x)
			size.x = ws.x+lh;
	}
	return size;
}

float Expander_view::Get_value(int id, const Widget& widget) const
{
	if(id == Expander::INDENT)
	{
		float sizey = al_get_font_line_height(font->Afont()) + 6;
		return sizey/4+6;
	}
	if(id == Expander::SELF_WIDTH)
	{
		const Expander& expander = dynamic_cast<const Expander&>(widget);
		const Ustring& text = expander.Get_text();
		float x = al_get_ustr_width(font->Afont(), text.Astring()) + 6;
		float y = al_get_font_line_height(font->Afont()) + 6;
		x += y/2;
		return x;
	}
	if(id == Expander::SELF_HEIGHT)
	{
		return al_get_font_line_height(font->Afont()) + 6;
	}
	return 0;
}

void Expander_view::Render(const Widget& widget) const
{
	const Expander& expander = dynamic_cast<const Expander&>(widget);

	Widget *child = expander.Get_child();
	if(child && expander.Is_open())
	{
		child->Render();
	}

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR tri_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
	ALLEGRO_COLOR select_color = al_map_rgb_f(0.0, 0, 0.8);

	const Ustring& text = expander.Get_text();
	float h = al_get_font_line_height(font->Afont());

	float text_width = al_get_ustr_width(font->Afont(), text.Astring());

	al_draw_ustr(font->Afont(), text_color, p.x+6+h/2, p.y+3, 0, text.Astring());
	float top = h*0.25;
	float middle = h*0.5;
	float bottom = h*0.75;

	if(expander.Is_open())
	{
		al_draw_filled_triangle(p.x+3, p.y+3+middle, p.x+3+middle, p.y+3+middle, p.x+3+top, p.y+3+bottom, tri_color);
		al_draw_triangle       (p.x+3, p.y+3+middle, p.x+3+middle, p.y+3+middle, p.x+3+top, p.y+3+bottom, edge_color, 0);
	}
	else
	{
		al_draw_filled_triangle(p.x+3, p.y+3+top, p.x+3+top, p.y+3+middle, p.x+3, p.y+3+bottom, tri_color);
		al_draw_triangle       (p.x+3, p.y+3+top, p.x+3+top, p.y+3+middle, p.x+3, p.y+3+bottom, edge_color, 0);
	}
}
