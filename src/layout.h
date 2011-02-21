#ifndef swag_layout_h
#define swag_layout_h

#include <string>
#include <map>
class Widget;
class Skin;

class Layout
{
public:
	Layout();
	~Layout();
	void Set_skin(Skin* skin);
	void Set_filename(const std::string& fn);
	const std::string& Get_filename() const;
	bool Load();
	bool Save() const;
	
	Widget* Get_root() const;
	std::string Add_widget(const std::string& name, Widget* widget, Widget* parent);
	void Delete_widget(const std::string& name);
	void Delete_widget(Widget* widget);
	void Remove_widget(Widget* widget);
	void Clear();
private:
	Layout(const Layout&){}
	const Layout& operator=(Layout&){}
	typedef std::map<std::string, Widget*> Name_to_widget;
	Name_to_widget name_to_widget;
	Widget* root;
	std::string filename;
	Skin* skin;
};

#endif
