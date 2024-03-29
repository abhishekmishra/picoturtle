#pragma once
#include <QString>
#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

namespace turtle
{
	// typedef void (*turtle_app_print_t)(QString);

	class TurtleController
	{
	private:
		// lua state object (see lua api docs)
		static lua_State *L;

		static bool handleLuaError(int luaErrorCode);
		static void turtle_init_cb(turtle::PicoTurtle *t, void *cb_args);
		static void turtle_update_cb(turtle::PicoTurtle *t, void *cb_args);
		static void turtle_paint_cb(turtle::PicoTurtle *t, void *cb_args);
		static void turtle_destroy_cb(turtle::PicoTurtle *t, void *cb_args);
		static void turtle_delay(turtle::PicoTurtle *t, int tm);

	public:
		static std::function<void(QString)> custom_lua_print_fn;
		static std::function<void(turtle::PicoTurtle *t)> notify_turtle_created_fn;
		static std::function<void(turtle::PicoTurtle *t)> notify_turtle_update_fn;
		static std::function<void(turtle::PicoTurtle *t)> notify_turtle_paint_fn;
		static std::function<void(turtle::PicoTurtle *t, int tm)> turtle_delay_fn;

		static void turtle_message(const QString &src, const QString &msg);
		static int init_turtle_lua_binding();
		static void cleanup_turtle_lua_binding();
		static void set_custom_lua_print_fn(std::function<void(QString)> printfn);
		static void set_notify_turtle_created_fn(std::function<void(turtle::PicoTurtle *t)> notifyfn);
		static void set_notify_turtle_update_fn(std::function<void(turtle::PicoTurtle *t)> notifyfn);
		static void set_notify_turtle_paint_fn(std::function<void(turtle::PicoTurtle *t)> paintfn);
		static void set_turtle_delay_fn(std::function<void(turtle::PicoTurtle *t, int tm)> delayfn);
		static int handle_turtle_lua_args(int argc, char *argv[]);
		static int run_lua_file(const char *filename);
		static int run_lua_script(const char *script);
	};
}
