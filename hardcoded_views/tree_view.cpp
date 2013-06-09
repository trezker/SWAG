#include "tree_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <tree.h>
#include "font.h"

Vector2 Tree_view::Request_size(const Widget& widget) const
{
	const Tree& tree = dynamic_cast<const Tree&>(widget);
	Vector2 size;
	const Ustring& text = tree.Get_text();
	size.x = al_get_ustr_width(font->Afont(), text.Astring()) + 6;
	size.y = al_get_font_line_height(font->Afont()) + 6;
	float lh = size.y/4+6;

	Trees children = tree.Get_childtrees();
	size.x += size.y/2;
	if(tree.Is_open())
	{
		for(Trees::iterator i = children.begin(); i != children.end(); ++i)
		{
			Vector2 ws = (*i)->Request_size();
			size.y += ws.y;
			if(ws.x+lh>size.x)
				size.x = ws.x+lh;
		}
	}
	return size;
}

float Tree_view::Get_value(int id, const Widget& widget) const
{
	if(id == Tree::INDENT)
	{
		float sizey = al_get_font_line_height(font->Afont()) + 6;
		return sizey/4+6;
	}
	if(id == Tree::SELF_WIDTH)
	{
		const Tree& tree = dynamic_cast<const Tree&>(widget);
		const Ustring& text = tree.Get_text();
		float x = al_get_ustr_width(font->Afont(), text.Astring()) + 6;
		float y = al_get_font_line_height(font->Afont()) + 6;
		x += y/2;
		return x;
	}
	if(id == Tree::SELF_HEIGHT)
	{
		return al_get_font_line_height(font->Afont()) + 6;
	}
	return 0;
}

void Tree_view::Render(const Widget& widget) const
{
	const Tree& tree = dynamic_cast<const Tree&>(widget);

	Trees children = tree.Get_childtrees();
	if(tree.Is_open())
	{
		for(Trees::iterator i = children.begin(); i != children.end(); ++i)
		{
			(*i)->Render();
		}
	}

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR tri_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);
	ALLEGRO_COLOR select_color = al_map_rgb_f(0.0, 0, 0.8);

	const Ustring& text = tree.Get_text();
	float h = al_get_font_line_height(font->Afont());

	float text_width = al_get_ustr_width(font->Afont(), text.Astring());
	if(tree.Is_selected())
		al_draw_filled_rectangle(p.x+3+h/2, p.y+1, p.x+6+h/2+text_width, p.y+h+6, select_color);

	al_draw_ustr(font->Afont(), text_color, p.x+6+h/2, p.y+3, 0, text.Astring());
	if(!children.empty())
	{
		float top = h*0.25;
		float middle = h*0.5;
		float bottom = h*0.75;

		if(tree.Is_open())
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
}
