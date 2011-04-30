#include "ustring.h"

Ustring::Ustring()
{
	ustr = al_ustr_new("");
}

Ustring::Ustring(const ALLEGRO_USTR* str)
{
	ustr = al_ustr_dup(str);
}

Ustring::Ustring(const Ustring& str)
{
	ustr = al_ustr_dup(str.ustr);
}

Ustring::Ustring(const char* s)
{
	ustr = al_ustr_new(s);
}

Ustring::Ustring(int c)
{
	ustr = al_ustr_new("");
	al_ustr_append_chr(ustr, c);
}

Ustring::~Ustring()
{
	al_ustr_free(ustr);
}

Ustring& Ustring::operator= (const ALLEGRO_USTR* str)
{
	al_ustr_assign(ustr, str);
	return *this;
}

Ustring& Ustring::operator= (const Ustring& str)
{
	al_ustr_assign(ustr, str.ustr);
	return *this;
}

Ustring& Ustring::operator= (const char* s)
{
	al_ustr_assign_cstr(ustr, s);
	return *this;
}

Ustring& Ustring::operator= (int32_t c)
{
	al_ustr_assign(ustr, al_ustr_empty_string());
	al_ustr_append_chr(ustr, c);
	return *this;
}

bool Ustring::operator== (const Ustring& str) const
{
	return al_ustr_equal(ustr, str.ustr);
}

bool Ustring::operator!= (const Ustring& str) const
{
	return !((*this)==str);
}

const char* Ustring::Cstring() const
{
	return al_cstr(ustr);
}

const ALLEGRO_USTR *Ustring::Astring() const
{
	return ustr;
}

ALLEGRO_USTR *Ustring::Astring()
{
	return ustr;
}

Ustring Ustring::Substring(int start_pos, int end_pos) const
{
	int offset_start_pos = al_ustr_offset(ustr, start_pos);
	int offset_end_pos = al_ustr_offset(ustr, end_pos);
	return al_ustr_dup_substr(ustr, offset_start_pos, offset_end_pos);
}

int Ustring::Length() const
{
	return al_ustr_length(ustr);
}
