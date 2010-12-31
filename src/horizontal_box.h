#ifndef swag_horizontal_box_h
#define swag_horizontal_box_h

#include "box.h"

class Horizontal_box: public Box
{
public:
	virtual Widget* Clone() const;
	virtual void Organise();
};

#endif
