#include "clipboard.h"
#include <cstdlib>
#include <fstream>

void Set_clipboard_text(const char* text)
{
	std::ofstream f(".clipboard");
	if(f.is_open())
	{
		f<<text;
	}
	f.close();
	system("cat .clipboard | xclip -selection c");
}

const char* Get_clipboard_text()
{
	system("xclip -o -selection c > .clipboard");
	std::string text;
	std::ifstream f(".clipboard");
	if(f.is_open())
	{
		char line[256];
		f.getline(line, 256);
		text = line;
		f.close();
	}
	return text.c_str();
}
