#ifndef swag_vertical_box_h
#define swag_vertical_box_h

#include "box.h"

class Vertical_box: public Box
{
public:
	virtual Widget* Clone() const;
	virtual void Organise();
};

#endif
