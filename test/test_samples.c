#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "picoturtle_lua.h"
#include "runtime.h"

static int run_sample(const char *path) {
    picoturtle_runtime_t runtime = {
        .canvas_width = PICOTURTLE_DEFAULT_CANVAS_WIDTH,
        .canvas_height = PICOTURTLE_DEFAULT_CANVAS_HEIGHT,
        .initialized = false
    };
    picoturtle_runtime_set_default(&runtime);

    lua_State *L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "Unable to create Lua state for %s.\n", path);
        return 1;
    }

    luaL_openlibs(L);
    luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
    lua_pop(L, 1);

    int status = luaL_dostring(L, "t = require('picoturtle').new()");
    if (status == LUA_OK) {
        status = luaL_dofile(L, path);
    }
    if (status != LUA_OK) {
        fprintf(stderr, "%s: %s\n", path, lua_tostring(L, -1));
    }

    lua_close(L);
    picoturtle_runtime_set_default(NULL);
    return status == LUA_OK ? 0 : 1;
}

int main(void) {
    const char *samples[] = {
        "lua/samples/turtle_basic.lua",
        "lua/samples/spiral.lua",
        "lua/samples/tree.lua",
        "lua/samples/sierpinski.lua",
        "lua/samples/polygons.lua",
        "lua/samples/save_restore.lua",
        "lua/samples/fern.lua",
        "lua/samples/text-demo.lua"
    };
    const size_t sample_count = sizeof(samples) / sizeof(samples[0]);

    for (size_t index = 0; index < sample_count; index++) {
        fprintf(stderr, "Running %s\n", samples[index]);
        fflush(stderr);
        if (run_sample(samples[index]) != 0) {
            return 1;
        }
    }
    return 0;
}
