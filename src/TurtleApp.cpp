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

#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#include "TurtleAppWindow.hpp"

#include "argparse.h"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

int init_turtle_lua_binding(lua_State** luaState);
int handle_turtle_lua_args(lua_State* L, int argc, char* argv[]);
int run_lua_file(lua_State* luaState, const char* filename);
int run_lua_script(lua_State* luaState, const char* script);
void cleanup_turtle_lua_binding(lua_State* luaState);
void turtle_init_cb(turtle::PicoTurtle* t, void* cb_args);
void turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args);

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

sk_sp<SkImage> img;

int main(int argc, char* argv[])
{
	// lua state object (see lua api docs)
	lua_State* L;

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

	turtle::PicoTurtle::set_init_callback(&turtle_init_cb, NULL);
	turtle::PicoTurtle::set_destroy_callback(&turtle_destroy_cb, NULL);

	// initialize the turtle lua binding with args
	init_turtle_lua_binding(&L);
	handle_turtle_lua_args(L, argc, argv);
	// cleanup the turtle lua binding
	cleanup_turtle_lua_binding(L);

	if (gui != 0)
	{
		gui_window(argc, argv);
	}

	return 0;
}

int gui_window(int argc, char* argv[])
{
	QApplication a(argc, argv);
	turtle::TurtleAppWindow mainWindow;
	mainWindow.show();
	return a.exec();
}

bool handleLuaError(lua_State* luaState, int luaErrorCode)
{
	if (luaErrorCode == LUA_OK)
	{
		return true;
	}
	else
	{
		const char* err_msg = lua_tostring(luaState, -1);
		std::cout << err_msg << std::endl;
		return false;
	}
}

int init_turtle_lua_binding(lua_State** luaState)
{
	// Create Lua State
	(*luaState) = luaL_newstate();
	lua_State* L = (*luaState);

	if (L == NULL)
	{
		return -1;
	}

	// Add standard libraries to Lua Virtual Machine
	luaL_openlibs(L);

	// picoturtle = require "picoturtle"
	luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
	lua_pop(L, 1); /* remove result from previous call */

	// TODO: Set path using optional args
	char* turtleLuaDir = getenv(TURTLE_LUA_DIR_ENV_VAR);
	if (turtleLuaDir == NULL || strlen(turtleLuaDir) == 0)
	{
		printf("Warning: %s is not set or empty!\n", TURTLE_LUA_DIR_ENV_VAR);
		turtleLuaDir = (char*)"lua";
	}
	char* setPathCodeStr = (char*)calloc(strlen(turtleLuaDir) + 1024, sizeof(char));
	if (setPathCodeStr == NULL)
	{
		printf("Fatal: Unable to alloc string to set load path in lua!\n");
	}

	sprintf(setPathCodeStr, "package.path = '%s/?.lua;?.lua;' .. package.path", turtleLuaDir);
	// printf("Setting path via code -> |%s|\n", setPathCodeStr);

	run_lua_script(L, setPathCodeStr);
	return 0;
}

int handle_turtle_lua_args(lua_State* L, int argc, char* argv[])
{
	const char* fname = NULL;

	lua_newtable(L);
	if (argc > 0)
	{
		fname = argv[0];
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				lua_pushstring(L, argv[i]);
				lua_seti(L, -2, i - 1);
			}
		}
		printf("input lua file - %s\n", fname);
	}
	lua_setglobal(L, "arg");

	if (fname != NULL)
	{
		run_lua_file(L, fname);
	}

	return 0;
}

int run_lua_file(lua_State* luaState, const char* filename)
{
	if (filename != NULL && strlen(filename) > 0)
	{
		if (handleLuaError(luaState, luaL_dofile(luaState, filename)))
		{
			printf("File execution complete -> %s. \n", filename);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int run_lua_script(lua_State* luaState, const char* script)
{
	if (script != NULL && strlen(script) > 0)
	{
		if (handleLuaError(luaState, luaL_dostring(luaState, script)))
		{
			// uncomment for debug only.
			// printf("Script execution complete.\n");
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

void cleanup_turtle_lua_binding(lua_State* luaState)
{
	lua_close(luaState);
}

void turtle_init_cb(turtle::PicoTurtle* t, void* cb_args)
{
	printf("PicoTurtle created - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
}

void turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args)
{
	printf("PicoTurtle destroyed - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
	img = t->getRasterSurface()->makeImageSnapshot();
	if (img)
	{
		printf("Image [%d x %d].\n", img->width(), img->height());
	}
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
