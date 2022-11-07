#pragma once
#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

namespace turtle {

	class TurtleController {
	public:
		TurtleController();
		~TurtleController();
		int handle_turtle_lua_args(int argc, char* argv[]);

	private:

		// lua state object (see lua api docs)
		lua_State* L;

		bool handleLuaError(int luaErrorCode);
		int init_turtle_lua_binding();
		void cleanup_turtle_lua_binding();
		static void turtle_init_cb(turtle::PicoTurtle* t, void* cb_args);
		static void turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args);

	public:
		int run_lua_file(const char* filename);
		int run_lua_script(const char* script);
	};
}
