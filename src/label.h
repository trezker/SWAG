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
	virtual sinxml::Element* To_xml() const;
	virtual void To_yaml(YAML::Emitter& out) const;
private:
	std::string text;
};

#endif
