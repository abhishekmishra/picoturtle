#pragma once
#include "PicoTurtle.hpp"

extern "C"
{

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_PICOTURTLE_OBJECT "PicoTurtle"

LUALIB_API int luaopen_picoturtle(lua_State* L);

} //end extern "C"