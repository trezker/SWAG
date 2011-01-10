#include "clipboard.h"
#include <cstdlib>
#include <fstream>

void Clipboard_copy_text(const char* text)
{
	std::string s = "echo ";
	s += text;
	s += " | xclip -selection c";
	system(s.c_str());
}

std::string Clipboard_paste_text()
{
	system("xclip -o -selection c > paste.txt");
	std::string text;
	std::ifstream f("paste.txt");
	if(f.is_open())
	{
		char line[256];
		f.getline(line, 256);
		text = line;
		f.close();
	}
	return text;
}
