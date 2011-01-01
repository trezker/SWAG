#ifndef swag_skin_h
#define swag_skin_h

#include <string>
#include <map>
#include <list>
#include "widget.h"

/* Class: Skin
 * */
class Skin
{
public:
	/* Destructor: ~Skin
	 * */
	virtual ~Skin();

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

	void Update(float t);
protected:
	/* Function: Set_prototype
	 * Maps the prototype to a name. If a prototype was already mapped to the given name, the old prototype is deleted.
	 * */
	void Set_prototype(const std::string& name, Widget* prototype);

	/* Function: Remove_prototype
	 * Removes a previously mapped prototype from the factory.
	 * */
	void Remove_prototype(const std::string& name);

	/* Function: Add_view
	 * Add a view to the list of views to be deleted when the skin is destroyed.
	 * */
	void Add_view(Widget_view* view);
	
	/* Function: Add_animated_view
	 * Add a view to the list of views to be updated
	 * */
	void Add_animated_view(Widget_view* view);
private:
	typedef std::map<std::string, Widget*> Prototypes;
	Prototypes prototypes;
	typedef std::list<Widget_view*> Views;
	Views views;
	Views animated_views;
};

#endif
