#pragma once
#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

namespace turtle {

	class TurtleController {
	private:
		// lua state object (see lua api docs)
		static lua_State* L;

		static bool handleLuaError(int luaErrorCode);
		static void turtle_init_cb(turtle::PicoTurtle* t, void* cb_args);
		static void turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args);

	public:
		static int init_turtle_lua_binding();
		static void cleanup_turtle_lua_binding();
		static int handle_turtle_lua_args(int argc, char* argv[]);
		static int run_lua_file(const char* filename);
		static int run_lua_script(const char* script);
	};
}
