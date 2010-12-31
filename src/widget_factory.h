#ifndef swag_widget_factory_h
#define swag_widget_factory_h

#include <string>
#include <map>
#include "widget.h"

/* Class: Widget_factory
 * */
class Widget_factory
{
public:
	/* Destructor: ~Widget_factory
	 * */
	~Widget_factory();

	/* Function: Set_prototype
	 * Maps the prototype to a name. If a prototype was already mapped to the given name, the old prototype is deleted.
	 * */
	void Set_prototype(const std::string& name, Widget* prototype);

	/* Function: Remove_prototype
	 * Removes a previously mapped prototype from the factory.
	 * */
	void Remove_prototype(const std::string& name);

	/* Function: Clone
	 * Clones the prototype that is mapped to name.
	 * If no prototype is mapped to name, this functions returns NULL.
	 * */
	template <typename T> T* Clone(const std::string& name)
	{
		Prototypes::iterator i = prototypes.find(name);
		if(i != prototypes.end())
		{
			return dynamic_cast<T*>(i->second->Clone());
		}
		return NULL;
	}
private:
	typedef std::map<std::string, Widget*> Prototypes;
	Prototypes prototypes;
};

#endif
