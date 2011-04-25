#ifndef swag_ustring_h
#define swag_ustring_h

#include <allegro5/allegro5.h>

class Ustring
{
public:
	Ustring();
	Ustring(const ALLEGRO_USTR* str);
	Ustring(const Ustring& str);
	Ustring(const char* s);
	Ustring(int c);
	~Ustring();
	Ustring& operator= (const ALLEGRO_USTR* str);
	Ustring& operator= (const Ustring& str);
	Ustring& operator= (const char* s);
	Ustring& operator= (int c);
	
	const char* Cstring() const;
	const ALLEGRO_USTR *Astring() const;
	ALLEGRO_USTR *Astring();
	
	Ustring Substring(int start_pos, int end_pos) const;
	int Length() const;
private:
	ALLEGRO_USTR *ustr;
};

#endif
