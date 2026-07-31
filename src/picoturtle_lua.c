#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "picoturtle_lua.h"
#include "turtle.h"
#include "image.h"

//------LUA BINDING--------

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
    picoturtle_turtle_t **turtle_ptr = (picoturtle_turtle_t **)luaL_checkudata(L, 1, LUA_PICOTURTLE_OBJECT);
    if (turtle_ptr && *turtle_ptr) {
        picoturtle_turtle_free_turtle(*turtle_ptr);
        *turtle_ptr = NULL;
    }
    return 0;
}

static int rl_trtl_new(lua_State *L)
{
    if (lua_gettop(L) != 0) {
        return luaL_error(L, "picoturtle.new expects no arguments");
    }
    picoturtle_turtle_t **turtle_ptr = (picoturtle_turtle_t **)lua_newuserdata(L, sizeof(picoturtle_turtle_t *));
    *turtle_ptr = NULL;
    picoturtle_turtle_make_turtle(turtle_ptr, "picoturtle", "picoturtle-1");
    if (*turtle_ptr == NULL)
    {
        return luaL_error(L, "Error creating PicoTurtle object.\n");
    }
    // set metatable of picoturtle object
    luaL_getmetatable(L, LUA_PICOTURTLE_OBJECT);
    lua_setmetatable(L, -2);
    return 1;
}

// --- Pure C turtle binding helpers ---
static picoturtle_turtle_t *lua_turtle_getobj(lua_State *L) {
    picoturtle_turtle_t **turtle_ptr = (picoturtle_turtle_t **)luaL_checkudata(L, 1, LUA_PICOTURTLE_OBJECT);
    picoturtle_turtle_t *t = *turtle_ptr;
    if (!t) luaL_typeerror(L, 1, LUA_PICOTURTLE_OBJECT);
    return t;
}

static picoturtle_turtle_state_t *lua_turtle_state_getobj(lua_State *L) {
    picoturtle_turtle_state_t **state_ptr = (picoturtle_turtle_state_t **)luaL_checkudata(
        L,
        1,
        LUA_TURTLE_STATE_OBJECT
    );
    if (state_ptr == NULL || *state_ptr == NULL) {
        luaL_typeerror(L, 1, LUA_TURTLE_STATE_OBJECT);
    }
    return *state_ptr;
}

static picoturtle_image_t *lua_turtle_image_getobj(lua_State *L, int index) {
    picoturtle_image_t **image_ptr = (picoturtle_image_t **)luaL_checkudata(
        L,
        index,
        LUA_TURTLE_IMAGE_OBJECT
    );
    if (image_ptr == NULL || *image_ptr == NULL) {
        luaL_typeerror(L, index, LUA_TURTLE_IMAGE_OBJECT);
    }
    return *image_ptr;
}

// --- Implemented functions ---
static int rl_turtle_penup(lua_State *L) {
    require_method_args(L, 0, "penup");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_pen_up(t);
    return 0;
}
static int rl_turtle_pendown(lua_State *L) {
    require_method_args(L, 0, "pendown");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_pen_down(t);
    return 0;
}
static int rl_turtle_forward(lua_State *L) {
    require_method_args(L, 1, "forward");
    float len = (float)luaL_checknumber(L, 2);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_forward(t, len);
    return 0;
}
static int rl_turtle_back(lua_State *L) {
    require_method_args(L, 1, "back");
    float len = (float)luaL_checknumber(L, 2);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_backward(t, len);
    return 0;
}
static int rl_turtle_left(lua_State *L) {
    require_method_args(L, 1, "left");
    float a = (float)luaL_checknumber(L, 2);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_left(t, a);
    return 0;
}
static int rl_turtle_right(lua_State *L) {
    require_method_args(L, 1, "right");
    float a = (float)luaL_checknumber(L, 2);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_right(t, a);
    return 0;
}
static int rl_turtle_setpos(lua_State *L) {
    require_method_args(L, 2, "setpos");
    float x = (float)luaL_checknumber(L, 2);
    float y = (float)luaL_checknumber(L, 3);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_set_position(t, x, y);
    return 0;
}
static int rl_turtle_getx(lua_State *L) {
    require_method_args(L, 0, "getx");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_location_t *loc = picoturtle_turtle_get_location(t);
    lua_pushnumber(L, picoturtle_turtle_location_get_x(loc));
    return 1;
}
static int rl_turtle_gety(lua_State *L) {
    require_method_args(L, 0, "gety");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_location_t *loc = picoturtle_turtle_get_location(t);
    lua_pushnumber(L, picoturtle_turtle_location_get_y(loc));
    return 1;
}
static int rl_turtle_setx(lua_State *L) {
    require_method_args(L, 1, "setx");
    float x = (float)luaL_checknumber(L, 2);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_set_x(t, x);
    return 0;
}
static int rl_turtle_sety(lua_State *L) {
    require_method_args(L, 1, "sety");
    float y = (float)luaL_checknumber(L, 2);
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_set_y(t, y);
    return 0;
}

static int rl_turtle_getwidth(lua_State *L) {
    require_method_args(L, 0, "getwidth");
    (void)lua_turtle_getobj(L);
    lua_pushinteger(L, picoturtle_turtle_get_canvas_width());
    return 1;
}

static int rl_turtle_setwidth(lua_State *L) {
    require_method_args(L, 1, "setwidth");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    int width = (int)luaL_checkinteger(L, 2);
    luaL_argcheck(L, width > 0, 2, "width must be greater than zero");
    if (!picoturtle_turtle_set_canvas_size(t, width, picoturtle_turtle_get_canvas_height())) {
        return luaL_error(L, "unable to resize the PicoTurtle canvas");
    }
    return 0;
}

static int rl_turtle_getheight(lua_State *L) {
    require_method_args(L, 0, "getheight");
    (void)lua_turtle_getobj(L);
    lua_pushinteger(L, picoturtle_turtle_get_canvas_height());
    return 1;
}

static int rl_turtle_setheight(lua_State *L) {
    require_method_args(L, 1, "setheight");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    int height = (int)luaL_checkinteger(L, 2);
    luaL_argcheck(L, height > 0, 2, "height must be greater than zero");
    if (!picoturtle_turtle_set_canvas_size(t, picoturtle_turtle_get_canvas_width(), height)) {
        return luaL_error(L, "unable to resize the PicoTurtle canvas");
    }
    return 0;
}

static int rl_turtle_penwidth(lua_State *L) {
    require_method_args(L, 1, "penwidth");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    float width = (float)luaL_checknumber(L, 2);
    luaL_argcheck(L, width > 0.0f, 2, "pen width must be greater than zero");
    picoturtle_turtle_pen_width(t, width);
    return 0;
}

static int check_colour_component(lua_State *L, int index) {
    lua_Integer component = luaL_checkinteger(L, index);
    luaL_argcheck(L, component >= 0 && component <= 255, index,
                  "colour component must be between 0 and 255");
    return (int)component;
}

static int rl_turtle_pencolor(lua_State *L) {
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;

    if (argument_count == 1) {
        const char *name = luaL_checkstring(L, 2);
        lua_pushinteger(L, picoturtle_turtle_colour(t, name));
        return 1;
    }
    if (argument_count == 3) {
        int red = check_colour_component(L, 2);
        int green = check_colour_component(L, 3);
        int blue = check_colour_component(L, 4);
        picoturtle_turtle_colour_rgba(
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
    picoturtle_turtle_home(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_clear(lua_State *L) {
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;

    if (argument_count == 0) {
        picoturtle_turtle_clear_canvas_colour(t, "white");
        return 0;
    }
    if (argument_count == 1) {
        picoturtle_turtle_clear_canvas_colour(t, luaL_checkstring(L, 2));
        return 0;
    }
    if (argument_count == 3) {
        Color color = {
            (unsigned char)check_colour_component(L, 2),
            (unsigned char)check_colour_component(L, 3),
            (unsigned char)check_colour_component(L, 4),
            255
        };
        picoturtle_runtime_clear(t->runtime, color);
        return 0;
    }
    return luaL_error(
        L,
        "clear expects no arguments, a colour name, or red, green, and blue values"
    );
}

static int rl_turtle_reset(lua_State *L) {
    require_method_args(L, 0, "reset");
    picoturtle_turtle_reset(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_heading(lua_State *L) {
    require_method_args(L, 1, "heading");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_heading(t, (double)luaL_checknumber(L, 2));
    return 0;
}

static int rl_turtle_canvas_size(lua_State *L) {
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    int argument_count = lua_gettop(L) - 1;

    if (argument_count == 2) {
        int width = (int)luaL_checkinteger(L, 2);
        int height = (int)luaL_checkinteger(L, 3);
        luaL_argcheck(L, width > 0, 2, "width must be greater than zero");
        luaL_argcheck(L, height > 0, 3, "height must be greater than zero");
        if (!picoturtle_turtle_set_canvas_size(t, width, height)) {
            return luaL_error(L, "unable to resize the PicoTurtle canvas");
        }
    } else if (argument_count != 0) {
        return luaL_error(L, "canvas_size expects either zero or two arguments");
    }

    lua_pushinteger(L, picoturtle_turtle_get_canvas_width());
    lua_pushinteger(L, picoturtle_turtle_get_canvas_height());
    return 2;
}

static int rl_turtle_drawme(lua_State *L) {
    require_method_args(L, 0, "drawme");
    picoturtle_turtle_draw_me(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_circle(lua_State *L) {
    require_method_args(L, 1, "circle");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    float radius = (float)luaL_checknumber(L, 2);
    luaL_argcheck(L, radius != 0.0f, 2, "radius must not be zero");
    picoturtle_turtle_circle(t, radius);
    return 0;
}

static int rl_turtle_arc(lua_State *L) {
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
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
    picoturtle_turtle_arc(t, radius, extent, steps);
    return 0;
}

static int rl_turtle_state(lua_State *L) {
    require_method_args(L, 0, "state");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_turtle_state_t *copy = NULL;
    if (!picoturtle_turtle_copy_state(t->current_state, &copy)) {
        return luaL_error(L, "unable to copy turtle state");
    }

    picoturtle_turtle_state_t **state_ptr = (picoturtle_turtle_state_t **)lua_newuserdata(
        L,
        sizeof(*state_ptr)
    );
    *state_ptr = copy;
    luaL_getmetatable(L, LUA_TURTLE_STATE_OBJECT);
    lua_setmetatable(L, -2);
    return 1;
}

static int rl_turtle_save(lua_State *L) {
    require_method_args(L, 0, "save");
    if (!picoturtle_turtle_save(lua_turtle_getobj(L))) {
        return luaL_error(L, "unable to save turtle state");
    }
    return 0;
}

static int rl_turtle_restore(lua_State *L) {
    require_method_args(L, 0, "restore");
    picoturtle_turtle_restore(lua_turtle_getobj(L));
    return 0;
}

static int rl_turtle_elapsed_time_ms(lua_State *L) {
    require_method_args(L, 0, "elapsed_time_ms");
    lua_pushinteger(
        L,
        (lua_Integer)picoturtle_turtle_elapsed_time_ms(lua_turtle_getobj(L))
    );
    return 1;
}

static int rl_turtle_export_img(lua_State *L) {
    require_method_args(L, 1, "export_img");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    const char *filename = luaL_checkstring(L, 2);
    if (!picoturtle_runtime_export_png(t->runtime, filename)) {
        return luaL_error(L, "unable to export canvas to %s", filename);
    }
    lua_pushstring(L, filename);
    return 1;
}

static int rl_turtle_font(lua_State *L) {
    require_method_args(L, 2, "font");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    const char *font_name = luaL_checkstring(L, 2);
    lua_Integer font_size = luaL_checkinteger(L, 3);
    luaL_argcheck(L, font_size > 0, 3, "font size must be greater than zero");
    picoturtle_turtle_set_font(t, font_name);
    picoturtle_turtle_set_font_size(t, (int)font_size);
    return 0;
}

static int rl_turtle_filltext(lua_State *L) {
    require_method_args(L, 1, "filltext");
    picoturtle_turtle_text(lua_turtle_getobj(L), luaL_checkstring(L, 2));
    return 0;
}

static int rl_turtle_stroketext(lua_State *L) {
    require_method_args(L, 1, "stroketext");
    picoturtle_turtle_text(lua_turtle_getobj(L), luaL_checkstring(L, 2));
    return 0;
}

static int rl_turtle_loadpic(lua_State *L) {
    require_method_args(L, 1, "loadpic");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    const char *path = luaL_checkstring(L, 2);
    picoturtle_image_t *image = picoturtle_image_load(t->runtime, path);
    if (image == NULL) {
        return luaL_error(L, "unable to load image %s", path);
    }

    picoturtle_image_t **image_ptr = (picoturtle_image_t **)lua_newuserdata(
        L,
        sizeof(*image_ptr)
    );
    *image_ptr = image;
    luaL_getmetatable(L, LUA_TURTLE_IMAGE_OBJECT);
    lua_setmetatable(L, -2);
    return 1;
}

static int rl_turtle_pic(lua_State *L) {
    require_method_args(L, 1, "pic");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);

    picoturtle_image_t **image_ptr = (picoturtle_image_t **)luaL_testudata(
        L,
        2,
        LUA_TURTLE_IMAGE_OBJECT
    );
    if (image_ptr != NULL && *image_ptr != NULL) {
        picoturtle_image_draw(
            *image_ptr,
            picoturtle_turtle_get_canvas_location_x(t),
            picoturtle_turtle_get_canvas_location_y(t)
        );
        return 0;
    }

    if (lua_type(L, 2) == LUA_TSTRING) {
        const char *path = lua_tostring(L, 2);
        picoturtle_image_t *image = picoturtle_image_load(t->runtime, path);
        if (image == NULL) {
            return luaL_error(L, "unable to load image %s", path);
        }
        picoturtle_image_draw(
            image,
            picoturtle_turtle_get_canvas_location_x(t),
            picoturtle_turtle_get_canvas_location_y(t)
        );
        picoturtle_image_destroy(image);
        return 0;
    }

    return luaL_error(L, "pic expects an image object or image path");
}

static int rl_turtle_image_free(lua_State *L) {
    picoturtle_image_t **image_ptr = (picoturtle_image_t **)luaL_checkudata(
        L,
        1,
        LUA_TURTLE_IMAGE_OBJECT
    );
    if (image_ptr != NULL && *image_ptr != NULL) {
        picoturtle_image_destroy(*image_ptr);
        *image_ptr = NULL;
    }
    return 0;
}

static int rl_turtle_image_tostring(lua_State *L) {
    require_method_args(L, 0, "__tostring");
    picoturtle_image_t *image = lua_turtle_image_getobj(L, 1);
    lua_pushfstring(L, "Turtle image [%s]", image->path);
    return 1;
}

static int rl_turtle_enable_update(lua_State *L) {
    require_method_args(L, 0, "enable_update");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_runtime_set_update_enabled(t->runtime, true);
    return 0;
}

static int rl_turtle_disable_update(lua_State *L) {
    require_method_args(L, 0, "disable_update");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    picoturtle_runtime_set_update_enabled(t->runtime, false);
    return 0;
}

static int rl_turtle_paint(lua_State *L) {
    require_method_args(L, 0, "paint");
    picoturtle_runtime_paint(lua_turtle_getobj(L)->runtime);
    return 0;
}

static int rl_turtle_delay(lua_State *L) {
    require_method_args(L, 1, "delay");
    picoturtle_turtle_t *t = lua_turtle_getobj(L);
    lua_Integer milliseconds = luaL_checkinteger(L, 2);
    luaL_argcheck(
        L,
        milliseconds >= 0,
        2,
        "delay must not be negative"
    );
    picoturtle_runtime_delay(t->runtime, (int)milliseconds);
    return 0;
}

static int rl_turtle_state_free(lua_State *L) {
    picoturtle_turtle_state_t **state_ptr = (picoturtle_turtle_state_t **)luaL_checkudata(
        L,
        1,
        LUA_TURTLE_STATE_OBJECT
    );
    if (state_ptr != NULL && *state_ptr != NULL) {
        picoturtle_turtle_free_state(*state_ptr);
        *state_ptr = NULL;
    }
    return 0;
}

static int rl_turtle_state_x(lua_State *L) {
    require_method_args(L, 0, "x");
    lua_pushnumber(L, lua_turtle_state_getobj(L)->location->x);
    return 1;
}

static int rl_turtle_state_y(lua_State *L) {
    require_method_args(L, 0, "y");
    lua_pushnumber(L, lua_turtle_state_getobj(L)->location->y);
    return 1;
}

static int rl_turtle_state_a(lua_State *L) {
    require_method_args(L, 0, "a");
    lua_pushinteger(L, lua_turtle_state_getobj(L)->pen_colour->a);
    return 1;
}

static int rl_turtle_state_r(lua_State *L) {
    require_method_args(L, 0, "r");
    lua_pushinteger(L, lua_turtle_state_getobj(L)->pen_colour->r);
    return 1;
}

static int rl_turtle_state_g(lua_State *L) {
    require_method_args(L, 0, "g");
    lua_pushinteger(L, lua_turtle_state_getobj(L)->pen_colour->g);
    return 1;
}

static int rl_turtle_state_b(lua_State *L) {
    require_method_args(L, 0, "b");
    lua_pushinteger(L, lua_turtle_state_getobj(L)->pen_colour->b);
    return 1;
}

static int rl_turtle_state_heading(lua_State *L) {
    require_method_args(L, 0, "heading");
    lua_pushnumber(L, lua_turtle_state_getobj(L)->heading);
    return 1;
}

static int rl_turtle_state_pd(lua_State *L) {
    require_method_args(L, 0, "pd");
    lua_pushboolean(L, lua_turtle_state_getobj(L)->pen_down);
    return 1;
}

static int rl_turtle_state_pw(lua_State *L) {
    require_method_args(L, 0, "pw");
    lua_pushnumber(L, lua_turtle_state_getobj(L)->pen_width);
    return 1;
}

static int rl_turtle_state_tostring(lua_State *L) {
    require_method_args(L, 0, "__tostring");
    picoturtle_turtle_state_t *state = lua_turtle_state_getobj(L);
    lua_pushfstring(
        L,
        "Turtle state [loc=(%f, %f), col=(%s, %d, %d, %d), "
        "pen(down=%d, width=%f)]",
        state->location->x,
        state->location->y,
        picoturtle_turtle_colour_get_name(state->pen_colour),
        state->pen_colour->r,
        state->pen_colour->g,
        state->pen_colour->b,
        state->pen_down,
        state->pen_width
    );
    return 1;
}

static const luaL_Reg PicoTurtle_funcs[] =
    {
        {"new", rl_trtl_new},
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
        {"export_img", rl_turtle_export_img},
        {"snap", rl_turtle_export_img},
        {"font", rl_turtle_font},
        {"filltext", rl_turtle_filltext},
        {"stroketext", rl_turtle_stroketext},
        {"canvas_size", rl_turtle_canvas_size},
        {"state", rl_turtle_state},
        {"save", rl_turtle_save},
        {"restore", rl_turtle_restore},
        {"elapsed_time_ms", rl_turtle_elapsed_time_ms},
        {"delay", rl_turtle_delay},
        {"paint", rl_turtle_paint},
        {"drawme", rl_turtle_drawme},
        {"circle", rl_turtle_circle},
        {"arc", rl_turtle_arc},
        {"enable_update", rl_turtle_enable_update},
        {"disable_update", rl_turtle_disable_update},
        {"loadpic", rl_turtle_loadpic},
        {"pic", rl_turtle_pic},
        {NULL, NULL}};

static const luaL_Reg TurtleState_meths[] =
    {
        {"__gc", rl_turtle_state_free},
        {"x", rl_turtle_state_x},
        {"y", rl_turtle_state_y},
        {"a", rl_turtle_state_a},
        {"r", rl_turtle_state_r},
        {"g", rl_turtle_state_g},
        {"b", rl_turtle_state_b},
        {"heading", rl_turtle_state_heading},
        {"hd", rl_turtle_state_heading},
        {"pd", rl_turtle_state_pd},
        {"pw", rl_turtle_state_pw},
        {NULL, NULL}};

static const luaL_Reg TurtleImage_meths[] =
    {
        {"__gc", rl_turtle_image_free},
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

    luaL_newmetatable(L, LUA_TURTLE_STATE_OBJECT);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, rl_turtle_state_tostring);
    lua_setfield(L, -2, "__tostring");
    luaL_setfuncs(L, TurtleState_meths, 0);
    lua_pop(L, 1);

    luaL_newmetatable(L, LUA_TURTLE_IMAGE_OBJECT);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, rl_turtle_image_tostring);
    lua_setfield(L, -2, "__tostring");
    luaL_setfuncs(L, TurtleImage_meths, 0);
    lua_pop(L, 1);

    // register functions - only turtle.new
    luaL_newlib(L, PicoTurtle_funcs);

    return 1;
}
