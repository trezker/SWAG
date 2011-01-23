#include "slider_box_view.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <slider_box.h>

Vector2 Slider_box_view::Request_size(const Widget& widget) const
{
	Vector2 size;
	const Slider_box& slider_box = dynamic_cast<const Slider_box&>(widget);
/*	Vector2 downsize(	Get_value(Slider_box::SLIDER_WIDTH, slider_box),
						Get_value(Slider_box::SLIDER_HEIGHT, slider_box));
	return slider_box.Get_child()->Request_size() + downsize;
*/
	return slider_box.Get_child()->Request_size();
}

float Slider_box_view::Get_value(int id, const Widget& widget) const
{
	if(Slider_box::SLIDER_WIDTH == id)
		return 20;
	if(Slider_box::SLIDER_HEIGHT == id)
		return 20;
	return 0;
}

void Slider_box_view::Render(const Widget& widget) const
{
	const Slider_box& slider_box = dynamic_cast<const Slider_box&>(widget);
	Widget* child = slider_box.Get_child();

	if(child)
	{
		int clip_x, clip_y, clip_w, clip_h;
		al_get_clipping_rectangle(&clip_x, &clip_y, &clip_w, &clip_h);
		Vector2 p = slider_box.Get_position();
		Vector2 s = slider_box.Get_size();
		Vector2 downsize(	Get_value(Slider_box::SLIDER_WIDTH, slider_box),
							Get_value(Slider_box::SLIDER_HEIGHT, slider_box));
		s -= downsize;
		al_set_clipping_rectangle(p.x, p.y, s.x, s.y);
		child->Render();
		al_set_clipping_rectangle(clip_x, clip_y, clip_w, clip_h);
		Vector2 ps = slider_box.Get_size();
		Vector2 cs = child->Get_size();
		if(cs.x>ps.x)
			hslider_view->Render(slider_box.Get_hslider());
		if(cs.y>ps.y)
			vslider_view->Render(slider_box.Get_vslider());
	}
}
