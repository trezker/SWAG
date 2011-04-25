#ifndef swag_text_interface_h
#define swag_text_interface_h

#include <string>
#include "ustring.h"

class Text_interface
{
public:
	virtual ~Text_interface(){}
	virtual void Set_text(const Ustring& t)=0;
};

#endif
