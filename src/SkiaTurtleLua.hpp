#pragma once
#include "SkiaTurtle.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define LUA_SKIA_TURTLE_OBJECT "SkiaTurtle"

int initTurtleLuaBinding(lua_State **luaState);
int runLuaFile(lua_State *luaState, const char *filename);
int runLuaScript(lua_State *luaState, const char *script);
void cleanupTurtleLuaBinding(lua_State *luaState);
