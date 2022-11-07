// see https://stackoverflow.com/a/38803842/9483968
// this is to stop sdl2 from trying to define its own main
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include <QApplication>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TurtleAppWindow.hpp"
#include "TurtleController.hpp"

#include "argparse.h"


/**
* Creates the Qt main window and runs the main loop for Qt.
* 
* @param argc program args
* @param argv program args
* @return return value from qt mainloop
*/
int gui_window(int argc, char* argv[]);

// https://stackoverflow.com/a/7776146/9483968
// Usage:
//     hex_dump(desc, addr, len, perLine);
//         desc:    if non-NULL, printed as a description before hex dump.
//         addr:    the address to start dumping from.
//         len:     the number of bytes to dump.
//         perLine: number of bytes on each output line.
void hex_dump(
	const char* desc,
	const void* addr,
	const int len,
	int perLine);

static const char* const usages[] = {
	"picoturtle [options] [[--] <turtle file> <turtle args>]",
	"picoturtle [options]",
	NULL,
};


int main(int argc, char* argv[])
{
	// option to indicate if app is to run in gui mode
	int gui = 0;

	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_GROUP("PicoTurtle Options"),
		OPT_BOOLEAN('g', "gui", &gui, "run in gui mode.", NULL, 0, 0),
		OPT_END(),
	};

	struct argparse argparse;
	argparse_init(&argparse, options, usages, 0);
	argparse_describe(&argparse, "\npicoturtle: A turtle programming environment.", "\nTODO: more info.");
	argc = argparse_parse(&argparse, argc, (const char**)argv);


	if (gui != 0)
	{
		gui_window(argc, argv);
	}

	return 0;
}

int gui_window(int argc, char* argv[])
{
	QApplication a(argc, argv);
	turtle::TurtleController *turtleController = new turtle::TurtleController();
	turtle::TurtleAppWindow mainWindow(turtleController);
	turtleController->handle_turtle_lua_args(argc, argv);
	mainWindow.show();
	return a.exec();
}


void hex_dump(
	const char* desc,
	const void* addr,
	const int len,
	int perLine)
{
	// Silently ignore silly per-line values.

	if (perLine < 4 || perLine > 64)
		perLine = 16;

	int i;
	unsigned char* buff = (unsigned char*)calloc(perLine + 1, sizeof(unsigned char));
	const unsigned char* pc = (const unsigned char*)addr;

	// Output description if given.

	if (desc != NULL)
		printf("%s:\n", desc);

	// Length checks.

	if (len == 0)
	{
		printf("  ZERO LENGTH\n");
		return;
	}
	if (len < 0)
	{
		printf("  NEGATIVE LENGTH: %d\n", len);
		return;
	}

	// Process every byte in the data.

	for (i = 0; i < len; i++)
	{
		// Multiple of perLine means new or first line (with line offset).

		if ((i % perLine) == 0)
		{
			// Only print previous-line ASCII buffer for lines beyond first.

			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset of current line.

			printf("  %04x ", i);
		}

		// Now the hex code for the specific character.

		printf(" %02x", pc[i]);

		// And buffer a printable ASCII character for later.

		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
			buff[i % perLine] = '.';
		else
			buff[i % perLine] = pc[i];
		buff[(i % perLine) + 1] = '\0';
	}

	// Pad out last line if not exactly perLine characters.

	while ((i % perLine) != 0)
	{
		printf("   ");
		i++;
	}

	// And print the final ASCII buffer.

	printf("  %s\n", buff);
}

// void turtle_example()
// {
// turtle::PicoTurtle *skTurtle = new turtle::PicoTurtle();

// skTurtle->pendown();
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->stop();

// skTurtle->export_img("out/out_raster.png");

// delete skTurtle;
// }
