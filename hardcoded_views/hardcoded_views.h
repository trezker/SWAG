#ifndef swag_hardcoded_views_h
#define swag_hardcoded_views_h

#include <swag.h>
#include <string>
#include <map>
#include <allegro5/allegro_font.h>

class Font;

class Hardcoded_skin: public Skin
{
public:
	Hardcoded_skin();
	virtual ~Hardcoded_skin();
	virtual bool Load();
	virtual void Save();
private:
	Font* font2;
	//ALLEGRO_FONT* font;
};

#endif
