#include <widget_view.h>
class Font;

class Button_view: public Widget_view
{
public:
	virtual Vector2 Request_size(const Widget& widget) const;
	virtual void Render(const Widget& widget) const;
public:
	Font* font;
};
