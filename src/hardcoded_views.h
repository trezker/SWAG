#ifndef swag_hardcoded_views_h
#define swag_hardcoded_views_h

#include "swag.h"
#include <string>
#include <map>

typedef std::map<std::string, Widget*> Widgetmap;

void Init_hardcoded_views(Widget_factory& widget_factory);

#endif
