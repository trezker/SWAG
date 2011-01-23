#ifndef swag_label_h
#define swag_label_h

#include "widget.h"
#include <string>
#include "text_interface.h"

class Label: public Widget, public Text_interface
{
public:
	Label();
	virtual Widget* Clone() const;
	virtual void Set_text(const std::string& t);
	std::string Get_text() const;
private:
	std::string text;
};

#endif
