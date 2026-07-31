#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "picoturtle_lua.h"

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
        "t:penup()\n"
        "t:setpos(0, 0)\n"
        "t:circle(25)\n"
        "assert(math.abs(t:getx()) < 0.001)\n"
        "assert(math.abs(t:gety()) < 0.001)\n"
        "t:heading(0)\n"
        "t:arc(25, 90, 12)\n"
        "assert(math.abs(t:getx()) > 0.001 or math.abs(t:gety()) > 0.001)\n"
        "t:clear()\n"
        "t:clear('white')\n"
        "t:clear(255, 255, 255)\n"
        "t:stop()\n"
        "t:penup()\n"
        "t:setpos(11, 22)\n"
        "t:pencolor('red')\n"
        "t:penwidth(6)\n"
        "local snapshot = t:state()\n"
        "assert(snapshot:x() == 11 and snapshot:y() == 22)\n"
        "assert(snapshot:r() == 255 and snapshot:g() == 0 and snapshot:b() == 0)\n"
        "assert(snapshot:a() == 255)\n"
        "assert(snapshot:pw() == 6 and snapshot:pd() == false)\n"
        "assert(type(snapshot:heading()) == 'number')\n"
        "assert(string.find(tostring(snapshot), 'Turtle state', 1, true))\n"
        "t:save()\n"
        "t:setpos(99, 88)\n"
        "t:pencolor('blue')\n"
        "t:restore()\n"
        "assert(t:getx() == 11 and t:gety() == 22)\n"
        "assert(snapshot:x() == 11 and snapshot:y() == 22)\n"
        "assert(t:elapsed_time_ms() >= 0)\n"
        "t:font('default', 24)\n"
        "t:filltext('hello')\n"
        "t:stroketext('world')\n"
        "t:disable_update()\n"
        "t:paint()\n"
        "t:enable_update()\n"
        "t:delay(0)\n"
        "assert(not pcall(function() t:export_img('headless.png') end))\n"
        "assert(not pcall(function() t:loadpic('missing.png') end))\n"
        "assert(not pcall(function() t:pic('missing.png') end))\n"
        "assert(not pcall(function() t:forward() end))\n"
        "assert(not pcall(function() t:forward(1, 2) end))\n"
        "assert(not pcall(function() t:penwidth(0) end))\n"
        "assert(not pcall(function() t:delay(-1) end))\n"
        "assert(not pcall(function() t:pencolor(256, 0, 0) end))\n"
        "assert(not pcall(function() t:canvas_size(100) end))\n";

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
