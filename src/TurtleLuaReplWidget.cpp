#include "TurtleLuaReplWidget.hpp"
#include <stdio.h>

using namespace turtle;

std::function<void(turtle::PicoTurtle* t)> TurtleLuaReplWidget::notify_turtle_created_fn = NULL;
std::function<void(turtle::PicoTurtle* t)> TurtleLuaReplWidget::notify_turtle_update_fn = NULL;
std::function<void(turtle::PicoTurtle* t)> TurtleLuaReplWidget::notify_turtle_paint_fn = NULL;
std::function<void(turtle::PicoTurtle* t, int tm)> TurtleLuaReplWidget::turtle_delay_fn = NULL;

TurtleLuaReplWidget::TurtleLuaReplWidget(QWidget* parent)
{
};

TurtleLuaReplWidget::~TurtleLuaReplWidget()
{
};

int TurtleLuaReplWidget::init_lua()
{
	LuaReplWidget::init_lua();
	init_turtle_lua_binding();
	return 0;
}

void TurtleLuaReplWidget::cleanup_lua()
{
	LuaReplWidget::cleanup_lua();

	// commented because this does same thing as the method above.
	//cleanup_turtle_lua_binding();
}

int TurtleLuaReplWidget::init_turtle_lua_binding()
{
	PicoTurtle::set_init_callback(&TurtleLuaReplWidget::turtle_init_cb, this);
	PicoTurtle::set_update_callback(&TurtleLuaReplWidget::turtle_update_cb, this);
	PicoTurtle::set_paint_callback(&TurtleLuaReplWidget::turtle_paint_cb, this);
	PicoTurtle::set_delay_callback(&TurtleLuaReplWidget::turtle_delay);
	PicoTurtle::set_destroy_callback(&TurtleLuaReplWidget::turtle_destroy_cb, this);

	// Add standard libraries to Lua Virtual Machine
	luaL_openlibs(L);

	// picoturtle = require "picoturtle"
	luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
	lua_pop(L, 1); /* remove result from previous call */

	// TODO: Set path using optional args
	char* turtleLuaDir = getenv(TURTLE_LUA_DIR_ENV_VAR);
	if (turtleLuaDir == NULL || strlen(turtleLuaDir) == 0)
	{
		// TurtleLuaReplWidget::turtle_message("app", "Warning: TURTLE_LUA_DIR_ENV_VAR is not set or empty!\n");
		turtleLuaDir = (char*)"lua";
	}

	size_t len_of_path_str = strlen(turtleLuaDir) + 1024;
	char* setPathCodeStr = (char*)calloc(len_of_path_str, sizeof(char));
	if (setPathCodeStr == NULL)
	{
		TurtleLuaReplWidget::print_to_repl("Fatal: Unable to alloc string to set load path in lua!\n");
		return -2;
	}

	snprintf(setPathCodeStr, len_of_path_str, "package.path = '%s/?.lua;?.lua;' .. package.path", turtleLuaDir);
	// for debug
	// TurtleLuaReplWidget::turtle_message("app", QString("Setting path via code -> |") + setPathCodeStr + "|");

	run_lua_script(setPathCodeStr);
	return 0;
}

void TurtleLuaReplWidget::cleanup_turtle_lua_binding()
{
	lua_close(L);
}

void TurtleLuaReplWidget::turtle_init_cb(turtle::PicoTurtle* t, void* cb_args)
{
	if (TurtleLuaReplWidget::notify_turtle_created_fn != NULL)
	{
		TurtleLuaReplWidget::notify_turtle_created_fn(t);
	}
	// TurtleLuaReplWidget::turtle_message("app", (QString("PicoTurtle created - Name: %1, Id: %2").arg(t->getName().c_str(), t->getId().c_str())));
}

void TurtleLuaReplWidget::turtle_update_cb(turtle::PicoTurtle* t, void* cb_args)
{
	if (TurtleLuaReplWidget::notify_turtle_update_fn != NULL)
	{
		TurtleLuaReplWidget::notify_turtle_update_fn(t);
	}
}

void TurtleLuaReplWidget::turtle_paint_cb(turtle::PicoTurtle* t, void* cb_args)
{
	if (TurtleLuaReplWidget::notify_turtle_paint_fn != NULL)
	{
		TurtleLuaReplWidget::notify_turtle_paint_fn(t);
	}
}

void TurtleLuaReplWidget::turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args)
{
	// sk_sp<SkImage> img;

	printf("PicoTurtle destroyed - Name: %s, Id: %s\n", t->get_name().c_str(), t->get_id().c_str());
	// img = t->getRasterSurface()->makeImageSnapshot();
	// if (img)
	//{
	//	TurtleLuaReplWidget::turtle_message("app", QString("Image [%1 x %2].\n").arg(img->width(), img->height()));
	// }
}

void TurtleLuaReplWidget::turtle_delay(turtle::PicoTurtle* t, int tm)
{
	if (TurtleLuaReplWidget::turtle_delay_fn != NULL)
	{
		TurtleLuaReplWidget::turtle_delay_fn(t, tm);
	}
}

void TurtleLuaReplWidget::set_notify_turtle_created_fn(std::function<void(turtle::PicoTurtle* t)> notifyfn)
{
	TurtleLuaReplWidget::notify_turtle_created_fn = notifyfn;
}

void TurtleLuaReplWidget::set_notify_turtle_update_fn(std::function<void(turtle::PicoTurtle* t)> notifyfn)
{
	TurtleLuaReplWidget::notify_turtle_update_fn = notifyfn;
}

void TurtleLuaReplWidget::set_notify_turtle_paint_fn(std::function<void(turtle::PicoTurtle* t)> paintfn)
{
	TurtleLuaReplWidget::notify_turtle_paint_fn = paintfn;
}

void TurtleLuaReplWidget::set_turtle_delay_fn(std::function<void(turtle::PicoTurtle* t, int tm)> delayfn)
{
	TurtleLuaReplWidget::turtle_delay_fn = delayfn;
}

bool TurtleLuaReplWidget::handleLuaError(int luaErrorCode)
{
	if (luaErrorCode == LUA_OK)
	{
		return true;
	}
	else
	{
		const char* err_msg = lua_tostring(L, -1);
		qDebug() << err_msg;
		TurtleLuaReplWidget::print_to_repl(err_msg);
		return false;
	}
}

int TurtleLuaReplWidget::run_lua_file(const char* filename)
{
	if (filename != NULL && strlen(filename) > 0)
	{
		if (handleLuaError(luaL_dofile(L, filename)))
		{
			// TurtleLuaReplWidget::turtle_message("app", QString("File execution complete -> ") + filename);
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int TurtleLuaReplWidget::run_lua_script(const char* script)
{
	// TurtleLuaReplWidget::turtle_message("app", QString("running lua script."));
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
