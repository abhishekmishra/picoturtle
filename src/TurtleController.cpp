#include <iostream>
#include <QDebug>
#include "TurtleController.hpp"

namespace turtle {
	lua_State* TurtleController::L = NULL;
	std::function<void(QString)> TurtleController::custom_lua_print_fn = NULL;

	void TurtleController::turtle_message(const QString& src, const QString& msg)
	{
		qDebug() << src + ":" << msg;
		if (TurtleController::custom_lua_print_fn != NULL)
		{
			TurtleController::custom_lua_print_fn(src + ": " + msg);
		}
	}

	bool TurtleController::handleLuaError(int luaErrorCode)
	{
		if (luaErrorCode == LUA_OK)
		{
			return true;
		}
		else
		{
			const char* err_msg = lua_tostring(L, -1);
			qDebug() << err_msg;
			TurtleController::custom_lua_print_fn(err_msg);
			return false;
		}
	}

	int print(lua_State* LUASTATE)
	{
		int nargs = lua_gettop(LUASTATE);
		QString input = "";
		for (int i = 1; i <= nargs; i++)
		{
			if (i > 1) input += " ";
			switch (lua_type(LUASTATE, i))
			{
			case LUA_TSTRING:
				input += lua_tostring(LUASTATE, i);
				break;
			case LUA_TNUMBER:
				input += QString::number((int)lua_tonumber(LUASTATE, i));
				break;
			case LUA_TBOOLEAN:
				input += QString::number(lua_toboolean(LUASTATE, i));
				break;
			}
		}

		TurtleController::turtle_message("program", input);
		return 0;
	}

	int TurtleController::init_turtle_lua_binding()
	{
		turtle::PicoTurtle::set_init_callback(&TurtleController::turtle_init_cb, NULL);
		turtle::PicoTurtle::set_destroy_callback(&TurtleController::turtle_destroy_cb, NULL);

		// Create Lua State
		L = luaL_newstate();

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
			TurtleController::turtle_message("app", "Warning: TURTLE_LUA_DIR_ENV_VAR is not set or empty!\n");
			turtleLuaDir = (char*)"lua";
		}
		char* setPathCodeStr = (char*)calloc(strlen(turtleLuaDir) + 1024, sizeof(char));
		if (setPathCodeStr == NULL)
		{
			TurtleController::turtle_message("app", "Fatal: Unable to alloc string to set load path in lua!\n");
			return -2;
		}

		sprintf(setPathCodeStr, "package.path = '%s/?.lua;?.lua;' .. package.path", turtleLuaDir);
		TurtleController::turtle_message("app", QString("Setting path via code -> |") + setPathCodeStr + "|");

		run_lua_script(setPathCodeStr);

		lua_pushcfunction(L, print);
		lua_setglobal(L, "turtle_print");
		TurtleController::turtle_message("app", "global function turtle_print is set");

		run_lua_script("turtle_print ('hello')");

		return 0;
	}

	int TurtleController::handle_turtle_lua_args(int argc, char* argv[])
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
			run_lua_file(fname);
		}

		return 0;
	}

	int TurtleController::run_lua_file(const char* filename)
	{
		if (filename != NULL && strlen(filename) > 0)
		{
			if (handleLuaError(luaL_dofile(L, filename)))
			{
				TurtleController::turtle_message("app", QString("File execution complete -> ") + filename);
			}
			return 0;
		}
		else
		{
			return -1;
		}
	}

	int TurtleController::run_lua_script(const char* script)
	{
		TurtleController::turtle_message("app", QString("running lua script."));
		if (script != NULL && strlen(script) > 0)
		{
			if (handleLuaError(luaL_dostring(L, script)))
			{
				// uncomment for debug only.
				// printf("Script execution complete.\n");
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return -1;
		}
	}

	void TurtleController::cleanup_turtle_lua_binding()
	{
		lua_close(L);
	}

	void TurtleController::turtle_init_cb(turtle::PicoTurtle* t, void* cb_args)
	{
		TurtleController::turtle_message("app", (QString("PicoTurtle created - Name: %1, Id: %2").arg(t->getName().c_str(), t->getId().c_str())));
	}

	void TurtleController::turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args)
	{
		sk_sp<SkImage> img;

		printf("PicoTurtle destroyed - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
		img = t->getRasterSurface()->makeImageSnapshot();
		if (img)
		{
			TurtleController::turtle_message("app", QString("Image [%d x %d].\n").arg(img->width(), img->height()));
		}
	}

	void TurtleController::set_custom_lua_print_fn(std::function<void(QString)> printfn)
	{
		TurtleController::custom_lua_print_fn = printfn;
	}
}
