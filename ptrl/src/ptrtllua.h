#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define MODULE_API_EXPORTS
#ifdef _WIN32
#  ifdef MODULE_API_EXPORTS
#    define MODULE_API __declspec(dllexport)
#  else
#    define MODULE_API __declspec(dllimport)
#  endif
#else
#  define MODULE_API
#endif

#define LUA_PICOTURTLE_OBJECT "PicoTurtle"
#define LUA_TURTLE_STATE_OBJECT "TurtleState"
#define LUA_TURTLE_IMAGE_OBJECT "TurtleImage"

MODULE_API int luaopen_picoturtle(lua_State* L);