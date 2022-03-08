#pragma once
#include "PicoTurtle.hpp"

extern "C"
{

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_PICOTURTLE_OBJECT "PicoTurtle"
#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

LUALIB_API int luaopen_picoturtle(lua_State* L);

}