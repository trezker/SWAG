#ifndef swag_hardcoded_views_h
#define swag_hardcoded_views_h

#include <swag.h>
#include <string>
#include <map>
#include <allegro5/allegro_font.h>

class Hardcoded_skin: public Skin
{
public:
	Hardcoded_skin();
	virtual ~Hardcoded_skin();
private:
	ALLEGRO_FONT* font;
};

#endif
