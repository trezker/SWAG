#ifndef swag_layout_h
#define swag_layout_h

#include <string>
#include <map>
#include "ustring.h"
class Widget;
class Skin;

typedef std::map<Ustring, Widget*> Name_to_widget;

class Layout
{
public:
	Layout();
	~Layout();
	void Set_skin(Skin* skin);
	void Set_filename(const Ustring& fn);
	const Ustring& Get_filename() const;
	bool Load_yaml();
	bool Save_yaml() const;
	
	Widget* Get_root() const;
	Ustring Add_widget(const Ustring& name, Widget* widget, Widget* parent);
	void Delete_widget(const Ustring& name);
	void Delete_widget(Widget* widget);
	void Remove_widget(Widget* widget);
	void Clear();
	const Name_to_widget& Get_widgets() const;
	Widget* Get_widget(const Ustring& name);
	bool Rename_widget(Widget* widget, const Ustring& name);
private:
	Layout(const Layout&){}
	const Layout& operator=(Layout&){}
	Name_to_widget name_to_widget;
	Widget* root;
	Ustring filename;
	Skin* skin;
};

#endif
