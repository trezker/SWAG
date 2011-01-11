#include "clipboard.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

// Temporarily...
#include <fstream>

#ifdef FILENAME_SIZE
    #error FILENAME_SIZE is already defined... :(
#endif

// 20 for filename, 1 for terminating character.
#define FILENAME_SIZE 21

#ifdef COMMAND_SIZE
    #error COMMAND_SIZE is already defined... :(
#endif

// 20 for filename, 25 for command line, 1 for terminating character.
#define COMMAND_SIZE 46

int Set_clipboard_text(const char * const text, const int size)
{
    char filename[FILENAME_SIZE] = "clipboard.tmp.XXXXXX";
    int fd = mkstemp(filename);

    if(!fd)
    {
        perror("Failed to open temp file");
        return 0;
    }

    FILE * file = fdopen(fd, "wD");

    if(!file)
    {
        perror("Failed to open file descriptor");
        return 0;
    }

    char * buffer = (char *)malloc(sizeof(char) * size);

    if(!buffer)
    {
        perror("Out of memory");
        return 0;
    }

    // Copy maximum of size bytes to buffer of known size.
    memcpy(buffer, text, size);

    // Ensure buffer is terminated.
    buffer[size] = '\0';

    fwrite(buffer, sizeof(char), strlen(buffer), file);

    free(buffer);

    fclose(file);

    char command[COMMAND_SIZE];

    snprintf(
            command,
            COMMAND_SIZE,
            "cat %s | xclip -selection c",
            filename);

    system(command);

    return 1;
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

