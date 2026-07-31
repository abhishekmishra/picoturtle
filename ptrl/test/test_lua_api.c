#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "ptrtllua.h"

static int run_api_test(lua_State *L) {
    const char *script =
        "local picoturtle = require('picoturtle')\n"
        "local t = picoturtle.new()\n"
        "assert(math.abs(t:getx()) < 0.001)\n"
        "assert(math.abs(t:gety()) < 0.001)\n"
        "t:forward(10)\n"
        "assert(math.abs(t:getx()) < 0.001)\n"
        "assert(math.abs(t:gety() - 10) < 0.001)\n"
        "t:right(90)\n"
        "t:fd(5)\n"
        "assert(math.abs(t:getx() - 5) < 0.001)\n"
        "assert(math.abs(t:gety() - 10) < 0.001)\n"
        "t:left(90)\n"
        "t:bk(2)\n"
        "assert(math.abs(t:getx() - 5) < 0.001)\n"
        "assert(math.abs(t:gety() - 8) < 0.001)\n"
        "t:penup()\n"
        "t:setpos(12, 34)\n"
        "assert(math.abs(t:getx() - 12) < 0.001)\n"
        "assert(math.abs(t:gety() - 34) < 0.001)\n"
        "t:setx(56)\n"
        "t:sety(78)\n"
        "assert(math.abs(t:getx() - 56) < 0.001)\n"
        "assert(math.abs(t:gety() - 78) < 0.001)\n"
        "t:pendown()\n"
        "assert(t:pencolor('red') == 1)\n"
        "assert(t:pencolor('not-a-real-colour') == 0)\n"
        "assert(t:pc(12, 34, 56) == 0)\n"
        "t:penwidth(3)\n"
        "t:pw(2)\n"
        "t:heading(0)\n"
        "t:penup()\n"
        "t:setpos(0, 0)\n"
        "t:forward(10)\n"
        "assert(math.abs(t:getx() - 10) < 0.001)\n"
        "assert(math.abs(t:gety()) < 0.001)\n"
        "t:home()\n"
        "assert(math.abs(t:getx()) < 0.001)\n"
        "assert(math.abs(t:gety()) < 0.001)\n"
        "t:reset()\n"
        "t:forward(10)\n"
        "assert(math.abs(t:getx()) < 0.001)\n"
        "assert(math.abs(t:gety() - 10) < 0.001)\n"
        "t:clear()\n"
        "t:clear('white')\n"
        "t:clear(255, 255, 255)\n"
        "t:stop()\n";

    int status = luaL_dostring(L, script);
    if (status != LUA_OK) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
    }
    return status;
}

int main(void) {
    lua_State *L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "Unable to create Lua state.\n");
        return 1;
    }

    luaL_openlibs(L);
    luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
    lua_pop(L, 1);

    int status = run_api_test(L);
    lua_close(L);
    return status == LUA_OK ? 0 : 1;
}
