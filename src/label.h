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
	virtual void Set_text(const Ustring& t);
	Ustring Get_text() const;
	virtual void To_yaml(YAML::Emitter& out) const;
	virtual void From_yaml(const YAML::Node& in);
private:
	Ustring text;
};

#endif
