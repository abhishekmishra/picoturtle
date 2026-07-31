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

static void require_method_args(
    lua_State *L,
    int expected,
    const char *method_name
) {
    int actual = lua_gettop(L) - 1;
    if (actual != expected) {
        luaL_error(
            L,
            "%s expects %d argument%s, received %d",
            method_name,
            expected,
            expected == 1 ? "" : "s",
            actual
        );
    }
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
    if (lua_gettop(L) != 0) {
        return luaL_error(L, "picoturtle.new expects no arguments");
    }
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
    trtl_t **turtle_ptr = (trtl_t **)luaL_checkudata(L, 1, LUA_PICOTURTLE_OBJECT);
    trtl_t *t = *turtle_ptr;
    if (!t) luaL_typeerror(L, 1, LUA_PICOTURTLE_OBJECT);
    return t;
}

// --- Implemented functions ---
static int rl_turtle_penup(lua_State *L) {
    require_method_args(L, 0, "penup");
    trtl_t *t = lua_turtle_getobj(L);
    trtl_pen_up(t);
    return 0;
}
static int rl_turtle_pendown(lua_State *L) {
    require_method_args(L, 0, "pendown");
    trtl_t *t = lua_turtle_getobj(L);
    trtl_pen_down(t);
    return 0;
}
static int rl_turtle_forward(lua_State *L) {
    require_method_args(L, 1, "forward");
    float len = (float)luaL_checknumber(L, 2);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_forward(t, len);
    return 0;
}
static int rl_turtle_back(lua_State *L) {
    require_method_args(L, 1, "back");
    float len = (float)luaL_checknumber(L, 2);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_backward(t, len);
    return 0;
}
static int rl_turtle_left(lua_State *L) {
    require_method_args(L, 1, "left");
    float a = (float)luaL_checknumber(L, 2);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_left(t, a);
    return 0;
}
static int rl_turtle_right(lua_State *L) {
    require_method_args(L, 1, "right");
    float a = (float)luaL_checknumber(L, 2);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_right(t, a);
    return 0;
}
static int rl_turtle_setpos(lua_State *L) {
    require_method_args(L, 2, "setpos");
    float x = (float)luaL_checknumber(L, 2);
    float y = (float)luaL_checknumber(L, 3);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_set_position(t, x, y);
    return 0;
}
static int rl_turtle_getx(lua_State *L) {
    require_method_args(L, 0, "getx");
    trtl_t *t = lua_turtle_getobj(L);
    trtl_location_t *loc = trtl_get_location(t);
    lua_pushnumber(L, trtl_location_get_x(loc));
    return 1;
}
static int rl_turtle_gety(lua_State *L) {
    require_method_args(L, 0, "gety");
    trtl_t *t = lua_turtle_getobj(L);
    trtl_location_t *loc = trtl_get_location(t);
    lua_pushnumber(L, trtl_location_get_y(loc));
    return 1;
}
static int rl_turtle_setx(lua_State *L) {
    require_method_args(L, 1, "setx");
    float x = (float)luaL_checknumber(L, 2);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_set_x(t, x);
    return 0;
}
static int rl_turtle_sety(lua_State *L) {
    require_method_args(L, 1, "sety");
    float y = (float)luaL_checknumber(L, 2);
    trtl_t *t = lua_turtle_getobj(L);
    trtl_set_y(t, y);
    return 0;
}

static int rl_turtle_getwidth(lua_State *L) {
    require_method_args(L, 0, "getwidth");
    (void)lua_turtle_getobj(L);
    lua_pushinteger(L, trtl_get_canvas_width());
    return 1;
}

static int rl_turtle_setwidth(lua_State *L) {
    require_method_args(L, 1, "setwidth");
    trtl_t *t = lua_turtle_getobj(L);
    int width = (int)luaL_checkinteger(L, 2);
    luaL_argcheck(L, width > 0, 2, "width must be greater than zero");
    if (!trtl_set_canvas_size(t, width, trtl_get_canvas_height())) {
        return luaL_error(L, "unable to resize the PicoTurtle canvas");
    }
    return 0;
}

static int rl_turtle_getheight(lua_State *L) {
    require_method_args(L, 0, "getheight");
    (void)lua_turtle_getobj(L);
    lua_pushinteger(L, trtl_get_canvas_height());
    return 1;
}

static int rl_turtle_setheight(lua_State *L) {
    require_method_args(L, 1, "setheight");
    trtl_t *t = lua_turtle_getobj(L);
    int height = (int)luaL_checkinteger(L, 2);
    luaL_argcheck(L, height > 0, 2, "height must be greater than zero");
    if (!trtl_set_canvas_size(t, trtl_get_canvas_width(), height)) {
        return luaL_error(L, "unable to resize the PicoTurtle canvas");
    }
    return 0;
}

static int rl_turtle_penwidth(lua_State *L) {
    require_method_args(L, 1, "penwidth");
    trtl_t *t = lua_turtle_getobj(L);
    float width = (float)luaL_checknumber(L, 2);
    luaL_argcheck(L, width > 0.0f, 2, "pen width must be greater than zero");
    trtl_pen_width(t, width);
    return 0;
}

static int check_colour_component(lua_State *L, int index) {
    lua_Integer component = luaL_checkinteger(L, index);
    luaL_argcheck(L, component >= 0 && component <= 255, index,
                  "colour component must be between 0 and 255");
    return (int)component;
}

static int rl_turtle_pencolor(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;

    if (argument_count == 1) {
        const char *name = luaL_checkstring(L, 2);
        lua_pushinteger(L, trtl_colour(t, name));
        return 1;
    }
    if (argument_count == 3) {
        int red = check_colour_component(L, 2);
        int green = check_colour_component(L, 3);
        int blue = check_colour_component(L, 4);
        trtl_colour_rgba(
            t,
            (uint8_t)red,
            (uint8_t)green,
            (uint8_t)blue,
            255
        );
        lua_pushinteger(L, 0);
        return 1;
    }
    return luaL_error(
        L,
        "pencolor expects a colour name or red, green, and blue values"
    );
}

static int rl_turtle_stop(lua_State *L) {
    require_method_args(L, 0, "stop");
    (void)lua_turtle_getobj(L);
    return 0;
}

static int rl_turtle_home(lua_State *L) {
    require_method_args(L, 0, "home");
    trtl_home(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_clear(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;

    if (argument_count == 0) {
        trtl_clear_canvas_colour(t, "white");
        return 0;
    }
    if (argument_count == 1) {
        trtl_clear_canvas_colour(t, luaL_checkstring(L, 2));
        return 0;
    }
    if (argument_count == 3) {
        Color color = {
            (unsigned char)check_colour_component(L, 2),
            (unsigned char)check_colour_component(L, 3),
            (unsigned char)check_colour_component(L, 4),
            255
        };
        ptrl_runtime_clear(t->runtime, color);
        return 0;
    }
    return luaL_error(
        L,
        "clear expects no arguments, a colour name, or red, green, and blue values"
    );
}

static int rl_turtle_reset(lua_State *L) {
    require_method_args(L, 0, "reset");
    trtl_reset(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_heading(lua_State *L) {
    require_method_args(L, 1, "heading");
    trtl_t *t = lua_turtle_getobj(L);
    trtl_heading(t, (double)luaL_checknumber(L, 2));
    return 0;
}

static int rl_turtle_canvas_size(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;

    if (argument_count == 2) {
        int width = (int)luaL_checkinteger(L, 2);
        int height = (int)luaL_checkinteger(L, 3);
        luaL_argcheck(L, width > 0, 2, "width must be greater than zero");
        luaL_argcheck(L, height > 0, 3, "height must be greater than zero");
        if (!trtl_set_canvas_size(t, width, height)) {
            return luaL_error(L, "unable to resize the PicoTurtle canvas");
        }
    } else if (argument_count != 0) {
        return luaL_error(L, "canvas_size expects either zero or two arguments");
    }

    lua_pushinteger(L, trtl_get_canvas_width());
    lua_pushinteger(L, trtl_get_canvas_height());
    return 2;
}

static int rl_turtle_drawme(lua_State *L) {
    require_method_args(L, 0, "drawme");
    trtl_draw_me(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_circle(lua_State *L) {
    require_method_args(L, 1, "circle");
    trtl_t *t = lua_turtle_getobj(L);
    float radius = (float)luaL_checknumber(L, 2);
    luaL_argcheck(L, radius != 0.0f, 2, "radius must not be zero");
    trtl_circle(t, radius);
    return 0;
}

static int rl_turtle_arc(lua_State *L) {
    trtl_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;
    if (argument_count < 1 || argument_count > 3) {
        return luaL_error(L, "arc expects radius, optional extent, and optional steps");
    }

    float radius = (float)luaL_checknumber(L, 2);
    float extent = argument_count >= 2
        ? (float)luaL_checknumber(L, 3)
        : -1.0f;
    int steps = argument_count == 3
        ? (int)luaL_checkinteger(L, 4)
        : -1;
    luaL_argcheck(L, radius != 0.0f, 2, "radius must not be zero");
    if (argument_count == 3) {
        luaL_argcheck(L, steps > 0, 4, "steps must be greater than zero");
    }
    trtl_arc(t, radius, extent, steps);
    return 0;
}

static const luaL_Reg PicoTurtle_funcs[] =
    {
        {"new", rl_trtl_new},
        // {"makegif", image_utils_make_gif},
        {NULL, NULL}};

// --- Update method table to use new C bindings and comment out unimplemented ones ---
static const luaL_Reg PicoTurtle_meths[] =
    {
        {"__gc", rl_trtl_free},
        {"getwidth", rl_turtle_getwidth},
        {"setwidth", rl_turtle_setwidth},
        {"getheight", rl_turtle_getheight},
        {"setheight", rl_turtle_setheight},
        {"penup", rl_turtle_penup},
        {"pu", rl_turtle_penup},
        {"pendown", rl_turtle_pendown},
        {"pd", rl_turtle_pendown},
        {"penwidth", rl_turtle_penwidth},
        {"pw", rl_turtle_penwidth},
        {"pencolor", rl_turtle_pencolor},
        {"pc", rl_turtle_pencolor},
        {"stop", rl_turtle_stop},
        {"home", rl_turtle_home},
        {"clear", rl_turtle_clear},
        {"reset", rl_turtle_reset},
        {"forward", rl_turtle_forward},
        {"fd", rl_turtle_forward},
        {"back", rl_turtle_back},
        {"bk", rl_turtle_back},
        {"setpos", rl_turtle_setpos},
        {"getx", rl_turtle_getx},
        {"gety", rl_turtle_gety},
        {"setx", rl_turtle_setx},
        {"sety", rl_turtle_sety},
        {"right", rl_turtle_right},
        {"rt", rl_turtle_right},
        {"left", rl_turtle_left},
        {"lt", rl_turtle_left},
        {"heading", rl_turtle_heading},
        // {"export_img", skia_turtle_export_img}, // TODO
        // {"snap", skia_turtle_export_img}, // TODO
        // {"font", skia_turtle_font}, // TODO
        // {"filltext", skia_turtle_filltext}, // TODO
        // {"stroketext", skia_turtle_stroketext}, // TODO
        {"canvas_size", rl_turtle_canvas_size},
        // {"state", skia_turtle_state}, // TODO
        // {"save", skia_turtle_save}, // TODO
        // {"restore", skia_turtle_restore}, // TODO
        // {"elapsed_time_ms", skia_turtle_elapsed_time_ms}, // TODO
        // {"delay", skia_turtle_delay}, // TODO
        // {"paint", skia_turtle_paint}, // TODO
        {"drawme", rl_turtle_drawme},
        {"circle", rl_turtle_circle},
        {"arc", rl_turtle_arc},
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
