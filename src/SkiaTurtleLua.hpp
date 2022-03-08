#pragma once
#include "SkiaTurtle.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_SKIA_TURTLE_OBJECT "SkiaTurtle"
#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

int initTurtleLuaBinding(lua_State **luaState, int argc, char *argv[]);
int runLuaFile(lua_State *luaState, const char *filename);
int runLuaScript(lua_State *luaState, const char *script);
void cleanupTurtleLuaBinding(lua_State *luaState);
}