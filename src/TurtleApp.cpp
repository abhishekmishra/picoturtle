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

int main(int argc, char* argv[])
{
	return gui_window(argc, argv);
}

int gui_window(int argc, char* argv[])
{
	QApplication a(argc, argv);
	// initialize the turtle lua binding with args
	turtle::TurtleController::init_turtle_lua_binding();

	// TODO: removed this for now - as this expect all non-lua
	// args to be removed before hand. Need to sanitize args
	// before passing on to a lua file with args.
	// this could be a common solution between a console program
	// and this one.
	// turtle::TurtleController::handle_turtle_lua_args(argc, argv);

	turtle::TurtleAppWindow *mainWindow = new turtle::TurtleAppWindow();

	// TODO: replace the print function to redirect output to console
	std::function<void(QString)> printfn = [=](QString input) {
		mainWindow->write_to_console(input);
	};

	turtle::TurtleController::set_custom_lua_print_fn(printfn);

	mainWindow->show();

	turtle::TurtleController::run_lua_script(
		"turtle_print('yo')\n"
		"turtle_print(1)\n"
		"local oldprint = print\n"
		"print = function(...)\n"
		"  -- do something with the args here.\n"
		"  turtle_print(...)\n"
		"end\n"
		"print ('hello from new print')\n"
	);

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
