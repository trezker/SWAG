#include "inputbox_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <inputbox.h>
#include <font.h>

Inputbox_view::Inputbox_view()
:cursor_flash(0)
{}

Vector2 Inputbox_view::Request_size(const Widget& widget) const
{
	const Inputbox& inputbox = dynamic_cast<const Inputbox&>(widget);
	const Ustring& text = inputbox.Get_text();
	Vector2 size;
	size.x = font->Get_ustr_width(text.Astring()) + 6;
	size.y = al_get_font_line_height(font->Afont()) + 6;
	return size;
}

float Inputbox_view::Get_value(int id, const Widget& widget) const
{
	const Inputbox& inputbox = dynamic_cast<const Inputbox&>(widget);
	const Ustring& text = inputbox.Get_text();
	ALLEGRO_MOUSE_STATE mouse;
	al_get_mouse_state(&mouse);
	int char_w = font->Get_ustr_width(" ");
	int x = mouse.x - widget.Get_position().x-6;
	int guess = x/char_w;
	int diff = x - font->Get_ustr_width(text.Substring(0, guess).Astring());
	//First back up if needed
	while(diff<0 && guess > 0)
	{
		--guess;
		diff = x - font->Get_ustr_width(text.Substring(0, guess).Astring());
	}
	//Then check forth
	int diff2;
	while(diff>0 && guess < text.Length())
	{
		diff2 = diff;
		++guess;
		diff = x - font->Get_ustr_width(text.Substring(0, guess).Astring());
	}
	if(diff>diff2)
		--guess;
	if(guess<0)
		guess=0;
	if(guess>text.Length())
		guess=text.Length();
	return guess;
}

void Inputbox_view::Set_value(int id, float v)
{
	cursor_flash=0;
}

void Inputbox_view::Render(const Widget& widget) const
{
	const Inputbox& inputbox = dynamic_cast<const Inputbox&>(widget);

	Vector2 p = widget.Get_position();
	Vector2 s = widget.Get_size();
	ALLEGRO_COLOR text_color = al_map_rgb_f(0, 0, 0);
	ALLEGRO_COLOR bg_color = al_map_rgb_f(1, 1, 1);
	ALLEGRO_COLOR edge_color = al_map_rgb_f(0.5, 0.5, 0.5);

	al_draw_filled_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, bg_color);
	al_draw_rectangle(p.x, p.y+1, p.x+s.x-1, p.y+s.y, edge_color, 0);

	int y = p.y + 3;
	int x = p.x + 3;
	const Ustring& text = inputbox.Get_text();
	
	if(inputbox.Has_focus())
	{
		int sel_s = inputbox.Get_selection_start();
		int sel_e = inputbox.Get_selection_end();

		int cp_s = font->Get_ustr_width(text.Substring(0, sel_s).Astring());
		int cp_e = font->Get_ustr_width(text.Substring(0, sel_e).Astring());
		int h = al_get_font_line_height(font->Afont());
		if(sel_s != sel_e)
			al_draw_filled_rectangle(x+cp_s-1, y, x+cp_e, y+h, al_map_rgb_f(0.5, 0.5, 1));
		else if(cursor_flash>0)
			al_draw_line(x+cp_e-1, y, x+cp_e, y+h, al_map_rgb_f(0, 0, 0), 0);
	}

	al_draw_ustr(font->Afont(), text_color, x, y, 0, text.Astring());
}

void Inputbox_view::Update(float t)
{
	cursor_flash+=t;
	if(cursor_flash>0.5)
		cursor_flash-=1;
}
