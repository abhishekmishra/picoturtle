#pragma once
#include "PicoTurtle.hpp"

extern "C"
{

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

MODULE_API int luaopen_picoturtle(lua_State* L);

} //end extern "C"
