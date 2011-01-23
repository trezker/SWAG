#ifndef swag_text_interface_h
#define swag_text_interface_h

#include <string>

class Text_interface
{
public:
	virtual ~Text_interface(){}
	virtual void Set_text(const std::string& t)=0;
};

#endif
