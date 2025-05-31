#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ptrtllua.h"
#include "turtle.h"

// TODO: uncomment when GifUtil.h is available
// #include "GifUtil.h"


//------LUA BINDING--------


static void stackDump(lua_State *L)
{
    int i;
    int top = lua_gettop(L); /* depth of the stack */
    for (i = 1; i <= top; i++)
    { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t)
        {
        case LUA_TSTRING:
        { /* strings */
            printf("'%s'", lua_tostring(L, i));
            break;
        }
        case LUA_TBOOLEAN:
        { /* Booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
        }
        case LUA_TNUMBER:
        { /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
        }
        default:
        { /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        }
        printf(" "); /* put a separator */
    }
    printf("\n"); /* end the listing */
}

static int rl_trtl_free(lua_State *L)
{
    trtl_t **turtle_ptr = (trtl_t **)luaL_checkudata(L, 1, LUA_PICOTURTLE_OBJECT);
    if (turtle_ptr && *turtle_ptr) {
        trtl_free_turtle(*turtle_ptr);
        *turtle_ptr = NULL;
    }
    return 0;
}

static int rl_trtl_new(lua_State *L)
{
    trtl_t **turtle_ptr = (trtl_t **)lua_newuserdata(L, sizeof(trtl_t *));
    *turtle_ptr = NULL;
    trtl_make_turtle(turtle_ptr, "picoturtle", "picoturtle-1");
    if (*turtle_ptr == NULL)
    {
        return luaL_error(L, "Error creating PicoTurtle object.\n");
    }
    // set metatable of picoturtle object
    luaL_getmetatable(L, LUA_PICOTURTLE_OBJECT);
    lua_setmetatable(L, -2);
    return 1;
}

// static int image_utils_make_gif(lua_State *L)
// {
//     const char *gif_fname = luaL_checkstring(L, lua_gettop(L));
//     lua_pop(L, 1);

//     if(lua_istable(L, lua_gettop(L))) {
//         int num_images = luaL_len(L, lua_gettop(L));
//         char** imgnames_arr = (char **)calloc(num_images, sizeof(char *));
//         if(imgnames_arr == NULL)
//         {
//             return luaL_error(L, "Error allocating array of img names.\n");
//         }

//         /* table is in the stack at index 't' */
//         lua_pushnil(L);  /* first key */

//         for (int i = 1; i < num_images + 1; i++)
//         {
//             if (lua_next(L, -2) != 0)
//             {
//                 const char *imgname = luaL_checkstring(L, -1);
//                 // printf("Found img %s\n", imgname);
//                 imgnames_arr[i - 1] = (char *)imgname;

//                 /* removes 'value'; keeps 'key' for next iteration */
//                 lua_pop(L, 1);
//             }
//             else
//             {
//                 return luaL_error(L, "image name #%d not found.\n", i);
//             }
//         }

//         /* pop the index for the lua_next call*/
//         lua_pop(L, 1);

//         /* pop the image names table */
//         lua_pop(L, 1);

//         init_gif_util();
//         make_gif_from_images(imgnames_arr, num_images, gif_fname);
//         exit_gif_util();
//     }

//     return 0;
// }

// --- Pure C turtle binding helpers ---
static trtl_t *lua_turtle_getobj(lua_State *L) {
    trtl_t **turtle_ptr = (trtl_t **)luaL_checkudata(L, lua_gettop(L), LUA_PICOTURTLE_OBJECT);
    trtl_t *t = *turtle_ptr;
    if (!t) luaL_typeerror(L, lua_gettop(L), LUA_PICOTURTLE_OBJECT);
    return t;
}

// --- Implemented functions ---
static int c_turtle_penup(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    trtl_pen_up(t);
    return 0;
}
static int c_turtle_pendown(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    trtl_pen_down(t);
    return 0;
}
static int c_turtle_forward(lua_State *L) {
    float len = luaL_checknumber(L, lua_gettop(L));
    trtl_t *t = lua_turtle_getobj(L);
    trtl_forward(t, len);
    return 0;
}
static int c_turtle_back(lua_State *L) {
    float len = luaL_checknumber(L, lua_gettop(L));
    trtl_t *t = lua_turtle_getobj(L);
    trtl_backward(t, len);
    return 0;
}
static int c_turtle_left(lua_State *L) {
    float a = luaL_checknumber(L, lua_gettop(L));
    trtl_t *t = lua_turtle_getobj(L);
    trtl_left(t, a);
    return 0;
}
static int c_turtle_right(lua_State *L) {
    float a = luaL_checknumber(L, lua_gettop(L));
    trtl_t *t = lua_turtle_getobj(L);
    trtl_right(t, a);
    return 0;
}
static int c_turtle_setpos(lua_State *L) {
    float y = luaL_checknumber(L, lua_gettop(L));
    float x = luaL_checknumber(L, lua_gettop(L) - 1);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_set_position(t, x, y);
    return 0;
}
static int c_turtle_getx(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    trtl_location_t *loc = trtl_get_location(t);
    lua_pushnumber(L, trtl_location_get_x(loc));
    return 1;
}
static int c_turtle_gety(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    trtl_location_t *loc = trtl_get_location(t);
    lua_pushnumber(L, trtl_location_get_y(loc));
    return 1;
}

// --- Not implemented in turtle.h ---
// TODO: Implement trtl_penwidth in turtle.h
// static int c_turtle_penwidth(lua_State *L) { ... }
// TODO: Implement trtl_pencolor in turtle.h
// static int c_turtle_pencolor(lua_State *L) { ... }
// TODO: Implement trtl_home in turtle.h
// static int c_turtle_home(lua_State *L) { ... }
// TODO: Implement trtl_clear in turtle.h
// static int c_turtle_clear(lua_State *L) { ... }
// TODO: Implement trtl_reset in turtle.h
// static int c_turtle_reset(lua_State *L) { ... }
// TODO: Implement trtl_heading in turtle.h
// static int c_turtle_heading(lua_State *L) { ... }
// TODO: Implement trtl_circle in turtle.h
// static int c_turtle_circle(lua_State *L) { ... }
// TODO: Implement trtl_arc in turtle.h
// static int c_turtle_arc(lua_State *L) { ... }

static const luaL_Reg PicoTurtle_funcs[] =
    {
        {"new", rl_trtl_new},
        // {"makegif", image_utils_make_gif},
        {NULL, NULL}};

// --- Update method table to use new C bindings and comment out unimplemented ones ---
static const luaL_Reg PicoTurtle_meths[] =
    {
        {"__gc", rl_trtl_free},
        // {"getwidth", skia_turtle_getwidth}, // TODO
        // {"setwidth", skia_turtle_setwidth}, // TODO
        // {"getheight", skia_turtle_getheight}, // TODO
        // {"setheight", skia_turtle_setheight}, // TODO
        {"penup", c_turtle_penup},
        {"pu", c_turtle_penup},
        {"pendown", c_turtle_pendown},
        {"pd", c_turtle_pendown},
        // {"penwidth", c_turtle_penwidth}, // TODO
        // {"pw", c_turtle_penwidth}, // TODO
        // {"pencolor", c_turtle_pencolor}, // TODO
        // {"pc", c_turtle_pencolor}, // TODO
        // {"stop", skia_turtle_stop}, // TODO
        // {"home", c_turtle_home}, // TODO
        // {"clear", c_turtle_clear}, // TODO
        // {"reset", c_turtle_reset}, // TODO
        {"forward", c_turtle_forward},
        {"fd", c_turtle_forward},
        {"back", c_turtle_back},
        {"bk", c_turtle_back},
        {"setpos", c_turtle_setpos},
        {"getx", c_turtle_getx},
        {"gety", c_turtle_gety},
        // {"setx", skia_turtle_setx}, // TODO
        // {"sety", skia_turtle_sety}, // TODO
        {"right", c_turtle_right},
        {"rt", c_turtle_right},
        {"left", c_turtle_left},
        {"lt", c_turtle_left},
        // {"heading", c_turtle_heading}, // TODO
        // {"export_img", skia_turtle_export_img}, // TODO
        // {"snap", skia_turtle_export_img}, // TODO
        // {"font", skia_turtle_font}, // TODO
        // {"filltext", skia_turtle_filltext}, // TODO
        // {"stroketext", skia_turtle_stroketext}, // TODO
        // {"canvas_size", skia_turtle_canvas_size}, // TODO
        // {"state", skia_turtle_state}, // TODO
        // {"save", skia_turtle_save}, // TODO
        // {"restore", skia_turtle_restore}, // TODO
        // {"elapsed_time_ms", skia_turtle_elapsed_time_ms}, // TODO
        // {"delay", skia_turtle_delay}, // TODO
        // {"paint", skia_turtle_paint}, // TODO
        // {"drawme", skia_turtle_drawme}, // TODO
        // {"circle", c_turtle_circle}, // TODO
        // {"arc", c_turtle_arc}, // TODO
        // {"enable_update", skia_turtle_enable_update}, // TODO
        // {"disable_update", skia_turtle_disable_update}, // TODO
        // {"loadpic", skia_turtle_loadpic}, // TODO
        // {"pic", skia_turtle_pic}, // TODO
        {NULL, NULL}};

int luaopen_picoturtle(lua_State *L)
{
    // create turtle metatable
    luaL_newmetatable(L, LUA_PICOTURTLE_OBJECT);

    // metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // register methods
    luaL_setfuncs(L, PicoTurtle_meths, 0);

    // TODO: TurtleState and TurtleImage metatables are commented out for now

    // register functions - only turtle.new
    luaL_newlib(L, PicoTurtle_funcs);

    return 1;
}
