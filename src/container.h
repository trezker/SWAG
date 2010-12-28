#ifndef swag_container_h
#define swag_container_h

#include "widget.h"

class Container: public Widget
{
public:
	virtual void Handle_child_resize();
};

#endif
