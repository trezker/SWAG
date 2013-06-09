#include "desktop_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <desktop.h>
#include "font.h"

Vector2 Desktop_view::Request_size(const Widget& widget) const
{
	Vector2 size;
	const Desktop& desktop = dynamic_cast<const Desktop&>(widget);
	return desktop.Get_child()->Request_size();
}

void Desktop_view::Render(const Widget& widget) const
{
	const Desktop& desktop = dynamic_cast<const Desktop&>(widget);
	Widget* child = desktop.Get_child();

	if(child)
		child->Render();

	const Widgets& top = desktop.Get_top();
	
	for(Widgets::const_iterator i = top.begin(); i != top.end(); ++i) {
		(*i)->Render();
	}

	Ustring tooltip = desktop.Get_tooltip(0,0);
	if(tooltip != "")
	{
		Vector2 p = desktop.Get_position();
		Vector2 s = desktop.Get_size();
		Vector2 tp = desktop.Get_tooltip_position() + Vector2(20, 10);
		ALLEGRO_COLOR text_color = al_map_rgb_f(0, 0, 0);
		ALLEGRO_COLOR bg_color = al_map_rgb_f(0.9, 0.9, 0.5);
		int w = al_get_text_width(font->Afont(), tooltip.Cstring());
		int h = al_get_font_line_height(font->Afont());
		if(tp.x+w > p.x+s.x)
		{
			tp.x = p.x+s.x - w;
			tp.y += 10;
			if(tp.y+h > p.y+s.y)
			{
				tp.y -= (20 + h);
			}
		}
		else if(tp.y+h > p.y+s.y)
		{
			tp.y = p.y+s.y - h;
		}

		al_draw_filled_rectangle(tp.x, tp.y, tp.x+w, tp.y+h, bg_color);
		al_draw_ustr(font->Afont(), text_color, tp.x, tp.y, 0, tooltip.Astring());
	}
}
