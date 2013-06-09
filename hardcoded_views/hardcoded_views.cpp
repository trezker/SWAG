#include "swag.h"
#include "hardcoded_views.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <yaml-cpp/yaml.h>
#include <fstream>

#include <iostream>

#include "size_mode_view.h"
#include "desktop_view.h"
#include "box_view.h"
#include "tree_view.h"
#include "expander_view.h"
#include "vertical_paned_view.h"
#include "vertical_slider_view.h"
#include "horizontal_paned_view.h"
#include "horizontal_slider_view.h"
#include "slider_box_view.h"
#include "button_view.h"
#include "checkbox_view.h"
#include "inputbox_view.h"
#include "label_view.h"
#include "menu_view.h"
#include "dropdown_menu_view.h"

#include "font.h"

Hardcoded_skin::Hardcoded_skin()
{
	
	//font = al_load_font("examples/data/times.ttf", 12, ALLEGRO_TTF_NO_KERNING);
	//font = al_load_font("examples/data/times.ttf", 12, 0);
	//font = al_load_font("examples/data/small_font.tga", 12, 0);
	font2 = new Font();
	//if(!font)
		//font = al_load_font("examples/data/times.ttf", 12, ALLEGRO_TTF_NO_KERNING);
		//font = al_load_font("examples/data/times.ttf", 12, 0);
		//font = al_load_font("examples/data/small_font.tga", 12, 0);
	if(!font2->Load("examples/data/small_font.tga", 12, 0))
		font2->Load("examples/data/small_font.tga", 12, 0);
	
	Widget* widget;

	Size_mode_view* size_mode_view = new Size_mode_view;
	size_mode_view->font = font2;
	widget = new Widget;
	widget->Set_view(size_mode_view);
	Set_prototype("size mode", widget);
	Add_view(size_mode_view);

	Horizontal_paned_view* hpaned_view = new Horizontal_paned_view;
	widget = new Horizontal_paned;
	widget->Set_view(hpaned_view);
	Set_prototype("horizontal paned", widget);
	Add_view(hpaned_view);

	Horizontal_slider_view* hslider_view = new Horizontal_slider_view;
	widget = new Horizontal_slider;
	widget->Set_view(hslider_view);
	Set_prototype("horizontal slider", widget);
	Add_view(hslider_view);

	Vertical_paned_view* vpaned_view = new Vertical_paned_view;
	widget = new Vertical_paned;
	widget->Set_view(vpaned_view);
	Set_prototype("vertical paned", widget);
	Add_view(vpaned_view);

	Vertical_slider_view* vslider_view = new Vertical_slider_view;
	widget = new Vertical_slider;
	widget->Set_view(vslider_view);
	Set_prototype("vertical slider", widget);
	Add_view(vslider_view);

	Slider_box_view* slider_box_view = new Slider_box_view;
	slider_box_view->hslider_view = hslider_view;
	slider_box_view->vslider_view = vslider_view;
	widget = new Slider_box;
	widget->Set_view(slider_box_view);
	Set_prototype("slider box", widget);
	Add_view(slider_box_view);

	Desktop_view* desktop_view = new Desktop_view;
	desktop_view->font = font2;
	widget = new Desktop;
	widget->Set_view(desktop_view);
	Set_prototype("desktop", widget);
	Add_view(desktop_view);

	Box_view* vbox_view = new Box_view;
	widget = new Vertical_box;
	widget->Set_view(vbox_view);
	Set_prototype("vertical box", widget);
	Add_view(vbox_view);

	HBox_view* hbox_view = new HBox_view;
	widget = new Horizontal_box;
	widget->Set_view(hbox_view);
	Set_prototype("horizontal box", widget);
	Add_view(hbox_view);

	Tree_view* tree_view = new Tree_view;
	tree_view->font = font2;
	widget = new Tree;
	widget->Set_view(tree_view);
	Set_prototype("tree", widget);
	Add_view(tree_view);

	Expander_view* expander_view = new Expander_view;
	expander_view->font = font2;
	widget = new Expander;
	widget->Set_view(expander_view);
	Set_prototype("expander", widget);
	Add_view(expander_view);

	Button_view* button_view = new Button_view;
	button_view->font = font2;
	widget = new Button;
	widget->Set_view(button_view);
	Set_prototype("button", widget);
	Add_view(button_view);

	Button_view* toggle_view = new Button_view;
	toggle_view->font = font2;
	widget = new Button;
	widget->Set_view(toggle_view);
	static_cast<Button*>(widget)->Set_toggle(true);
	Set_prototype("toggle button", widget);
	Add_view(toggle_view);

	Checkbox_view* checkbox_view = new Checkbox_view;
	checkbox_view->font = font2;
	widget = new Button;
	widget->Set_view(checkbox_view);
	static_cast<Button*>(widget)->Set_toggle(true);
	Set_prototype("checkbox", widget);
	Add_view(checkbox_view);

	Label_view* label_view = new Label_view;
	label_view->font = font2;
	widget = new Label;
	widget->Set_view(label_view);
	Set_prototype("label", widget);
	Add_view(label_view);

	Inputbox_view* inputbox_view = new Inputbox_view;
	inputbox_view->font = font2;
	widget = new Inputbox;
	widget->Set_view(inputbox_view);
	Set_prototype("inputbox", widget);
	Add_view(inputbox_view);
	Add_animated_view(inputbox_view);

	Menu_view* menu_view = new Menu_view;
	menu_view->font = font2;
	widget = new Menu;
	widget->Set_view(menu_view);
	Set_prototype("menu", widget);
	Add_view(menu_view);

	Dropdown_menu_view* dropdown_menu_view = new Dropdown_menu_view;
	dropdown_menu_view->font = font2;
	widget = new Dropdown_menu;
	widget->Set_view(dropdown_menu_view);
	Set_prototype("dropdown menu", widget);
	Add_view(dropdown_menu_view);
}

Hardcoded_skin::~Hardcoded_skin()
{
	//al_destroy_font(font);
	delete font2;
}

bool Hardcoded_skin::Load()
{
	if(Get_filename()=="")
	{
		std::cout<<"No filename"<<std::endl;
		return false;
	}
	try
	{
		std::ifstream fin(Get_filename().Cstring());
		YAML::Parser parser(fin);

		YAML::Node doc;
		parser.GetNextDocument(doc);
	}
	catch(YAML::Exception& e){
		std::cout<<"ERROR: "<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Caught error"<<std::endl;
	}
}

void Hardcoded_skin::Save()
{
}
