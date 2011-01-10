#ifndef swag_clipboard_h
#define swag_clipboard_h
#include <string>

void Clipboard_copy_text(const char* text);
std::string Clipboard_paste_text();

#endif
