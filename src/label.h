#ifndef swag_label_h
#define swag_label_h

#include "widget.h"
#include <string>

class Label: public Widget
{
public:
	virtual Widget* Clone() const;
	void Set_text(const std::string& t);
	std::string Get_text() const;
private:
	std::string text;
};

#endif
