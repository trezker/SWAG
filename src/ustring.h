#ifndef swag_ustring_h
#define swag_ustring_h

#include <allegro5/allegro5.h>
#include <ostream>
#include <istream>
#include <yaml-cpp/yaml.h>

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
	
	Ustring& operator+= (const Ustring& str);
	const Ustring operator+ (const Ustring& str) const;

	int Compare(const Ustring& str) const;
	bool operator== (const Ustring& str) const;
	bool operator!= (const Ustring& str) const;
	bool operator< (const Ustring& str) const;
	bool operator> (const Ustring& str) const;
	bool operator<= (const Ustring& str) const;
	bool operator>= (const Ustring& str) const;

	
	const char* Cstring() const;
	const ALLEGRO_USTR *Astring() const;
	ALLEGRO_USTR *Astring();
	
	Ustring Substring(int start_pos, int end_pos) const;
	int Length() const;
private:
	ALLEGRO_USTR *ustr;
};

std::ostream& operator<< (std::ostream& os, const Ustring& str);
void operator>> (std::istream& os, Ustring& str);

YAML::Emitter& operator<< (YAML::Emitter& os, const Ustring& str);
void operator>>(const YAML::Node& node, Ustring& str);

#endif
