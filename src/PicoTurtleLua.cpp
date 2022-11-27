#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PicoTurtleLua.hpp"
#include "PicoTurtle.hpp"

//------LUA BINDING--------

using namespace turtle;

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

static int skia_turtle_free(lua_State *L)
{
    delete *static_cast<PicoTurtle **>(luaL_checkudata(L, 1, LUA_PICOTURTLE_OBJECT));
    return 0;
}

static int skia_turtle_new(lua_State *L)
{
    PicoTurtle *t = new PicoTurtle();
    *static_cast<PicoTurtle **>(lua_newuserdata(L, sizeof(PicoTurtle *))) = t;

    // set metatable of picoturtle object
    luaL_getmetatable(L, LUA_PICOTURTLE_OBJECT);
    lua_setmetatable(L, -2);

    return 1;
}

static PicoTurtle *skia_turtle_getobj(lua_State *L)
{
    int top = lua_gettop(L);
    PicoTurtle *t = *static_cast<PicoTurtle **>(luaL_checkudata(L, top, LUA_PICOTURTLE_OBJECT));
    if (t == NULL)
    {
        luaL_typeerror(L, top, LUA_PICOTURTLE_OBJECT);
    }
    lua_pop(L, 1);
    return t;
}

static TurtleState *turtle_state_getobj(lua_State *L)
{
    int top = lua_gettop(L);
    TurtleState *state = *static_cast<TurtleState **>(luaL_checkudata(L, top, LUA_TURTLE_STATE_OBJECT));
    if (state == NULL)
    {
        luaL_typeerror(L, top, LUA_TURTLE_STATE_OBJECT);
    }
    lua_pop(L, 1);
    return state;
}

static int skia_turtle_getwidth(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    lua_pushinteger(L, t->get_canvas()->get_width());
    return 1;
}

static int skia_turtle_setwidth(lua_State *L)
{
    int width = luaL_checkinteger(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->get_canvas()->set_width(width);
    return 0;
}

static int skia_turtle_getheight(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    lua_pushinteger(L, t->get_canvas()->get_height());
    return 1;
}

static int skia_turtle_setheight(lua_State *L)
{
    int height = luaL_checkinteger(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->get_canvas()->set_height(height);
    return 0;
}

static int skia_turtle_penup(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    t->penup();
    return 0;
}

static int skia_turtle_pendown(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    t->pendown();
    return 0;
}

static int skia_turtle_penwidth(lua_State *L)
{
    float width = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->penwidth(width);
    return 0;
}

static int skia_turtle_pencolor(lua_State *L)
{
    int top = lua_gettop(L);
    int t = lua_type(L, top);
    if (t == LUA_TSTRING)
    {
        const char *color = luaL_checkstring(L, lua_gettop(L));
        lua_pop(L, 1);
        PicoTurtle *t = skia_turtle_getobj(L);
        lua_pushinteger(L, t->pencolor(color));
    }
    else
    {
        unsigned int b = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
        lua_pop(L, 1);
        unsigned int g = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
        lua_pop(L, 1);
        unsigned int r = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
        lua_pop(L, 1);

        PicoTurtle *t = skia_turtle_getobj(L);
        t->pencolor(r, g, b);
        lua_pushinteger(L, 0);
    }
    return 1;
}

static int skia_turtle_stop(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    t->stop();
    return 0;
}

static int skia_turtle_home(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    t->home();
    return 0;
}

static int skia_turtle_clear(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    t->get_canvas()->clear();
    return 0;
}

static int skia_turtle_reset(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    t->reset();
    return 0;
}

static int skia_turtle_forward(lua_State *L)
{
    float len = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->forward(len);
    return 0;
}

static int skia_turtle_back(lua_State *L)
{
    float len = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->back(len);
    return 0;
}

static int skia_turtle_setpos(lua_State *L)
{
    float y = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);
    float x = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->setpos(x, y);
    return 0;
}

static int skia_turtle_getx(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    lua_pushnumber(L, t->getx());
    return 1;
}

static int skia_turtle_gety(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    lua_pushnumber(L, t->gety());
    return 1;
}

static int skia_turtle_setx(lua_State *L)
{
    float x = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->setx(x);
    return 0;
}

static int skia_turtle_sety(lua_State *L)
{
    float y = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->sety(y);
    return 0;
}

static int skia_turtle_left(lua_State *L)
{
    float a = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->left(a);
    return 0;
}

static int skia_turtle_right(lua_State *L)
{
    float a = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->right(a);
    return 0;
}

static int skia_turtle_heading(lua_State *L)
{
    float a = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->heading(a);
    return 0;
}

static int skia_turtle_export_img(lua_State *L)
{
    const char *s = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->get_canvas()->export_img(s);
    return 0;
}

static int skia_turtle_font(lua_State *L)
{
    unsigned int sz = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    const char *str = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->font(str, sz);
    return 0;
}

static int skia_turtle_filltext(lua_State *L)
{
    const char *s = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->filltext(s);
    return 0;
}

static int skia_turtle_stroketext(lua_State *L)
{
    const char *s = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->stroketext(s);
    return 0;
}

static int skia_turtle_canvas_size(lua_State *L)
{
    int height = (int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    int width = (int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->canvas_size(width, height);
    return 0;
}

static int skia_turtle_state(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    TurtleState *state = t->get_current_state();

    *static_cast<TurtleState **>(lua_newuserdata(L, sizeof(TurtleState *))) = state;

    // set metatable of turtle state
    luaL_getmetatable(L, LUA_TURTLE_STATE_OBJECT);
    lua_setmetatable(L, -2);

    return 1;
}

static int skia_turtle_save(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);

    t->save();

    return 0;
}

static int skia_turtle_restore(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);

    t->restore();

    return 0;
}

static int skia_turtle_elapsed_time_ms(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);
    lua_pushnumber(L, t->elapsed_time_ms());
    return 1;
}

static int skia_turtle_delay(lua_State *L)
{
    int tm = (int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    PicoTurtle *t = skia_turtle_getobj(L);

    t->delay_ms(tm);

    return 0;
}

static int skia_turtle_paint(lua_State *L)
{
    PicoTurtle *t = skia_turtle_getobj(L);

    t->paint();

    return 0;
}

static int turtle_state_free(lua_State *L)
{
    delete *static_cast<TurtleState **>(luaL_checkudata(L, 1, LUA_TURTLE_STATE_OBJECT));
    return 0;
}

static int turtle_state_x(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_location()->getX());
    return 1;
}

static int turtle_state_y(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_location()->getY());
    return 1;
}

static int turtle_state_a(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_pen_color()->get_a());
    return 1;
}

static int turtle_state_r(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_pen_color()->get_r());
    return 1;
}

static int turtle_state_g(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_pen_color()->get_g());
    return 1;
}

static int turtle_state_b(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_pen_color()->get_b());
    return 1;
}

static int turtle_state_heading(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_heading());
    return 1;
}

static int turtle_state_pd(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushboolean(L, state->is_pen_down());
    return 1;
}

static int turtle_state_pw(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushnumber(L, state->get_pen_width());
    return 1;
}

static int turtle_state_tostring(lua_State *L)
{
    TurtleState *state = turtle_state_getobj(L);
    lua_pushfstring(L,
                    "Turtle state [loc=(%f, %f), col=(%s, %d, %d, %d), pen(down=%d, width=%f)",
                    state->get_location()->getX(),
                    state->get_location()->getY(),
                    state->get_pen_color()->get_color_name().c_str(),
                    state->get_pen_color()->get_r(),
                    state->get_pen_color()->get_g(),
                    state->get_pen_color()->get_b(),
                    state->is_pen_down(),
                    state->get_pen_width());
    return 1;
}

static const luaL_Reg PicoTurtle_funcs[] =
    {
        {"new", skia_turtle_new},
        {NULL, NULL}};

static const luaL_Reg PicoTurtle_meths[] =
    {
        {"__gc", skia_turtle_free},
        {"getwidth", skia_turtle_getwidth},
        {"setwidth", skia_turtle_setwidth},
        {"getheight", skia_turtle_getheight},
        {"setheight", skia_turtle_setheight},
        {"penup", skia_turtle_penup},
        {"pu", skia_turtle_penup},
        {"pendown", skia_turtle_pendown},
        {"pd", skia_turtle_pendown},
        {"penwidth", skia_turtle_penwidth},
        {"pencolor", skia_turtle_pencolor},
        {"stop", skia_turtle_stop},
        {"home", skia_turtle_home},
        {"clear", skia_turtle_clear},
        {"reset", skia_turtle_reset},
        {"forward", skia_turtle_forward},
        {"fd", skia_turtle_forward},
        {"back", skia_turtle_back},
        {"bk", skia_turtle_back},
        {"setpos", skia_turtle_setpos},
        {"getx", skia_turtle_getx},
        {"gety", skia_turtle_gety},
        {"setx", skia_turtle_setx},
        {"sety", skia_turtle_sety},
        {"right", skia_turtle_right},
        {"rt", skia_turtle_right},
        {"left", skia_turtle_left},
        {"lt", skia_turtle_left},
        {"heading", skia_turtle_heading},
        {"export_img", skia_turtle_export_img},
        {"font", skia_turtle_font},
        {"filltext", skia_turtle_filltext},
        {"stroketext", skia_turtle_stroketext},
        {"canvas_size", skia_turtle_canvas_size},
        {"state", skia_turtle_state},
        {"save", skia_turtle_save},
        {"restore", skia_turtle_restore},
        {"elapsed_time_ms", skia_turtle_elapsed_time_ms},
        {"delay", skia_turtle_delay},
        {"paint", skia_turtle_paint},
        {NULL, NULL}};

static const luaL_Reg TurtleState_meths[] =
    {
        {"__gc", turtle_state_free},
        {"x", turtle_state_x},
        {"y", turtle_state_y},
        {"a", turtle_state_a},
        {"r", turtle_state_r},
        {"g", turtle_state_g},
        {"b", turtle_state_b},
        {"heading", turtle_state_heading},
        {"hd", turtle_state_heading},
        {"pd", turtle_state_pd},
        {"pw", turtle_state_pw},
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

    // create turtle state metatable
    luaL_newmetatable(L, LUA_TURTLE_STATE_OBJECT);

    // metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, turtle_state_tostring);
    lua_setfield(L, -2, "__tostring");

    // register methods
    luaL_setfuncs(L, TurtleState_meths, 0);

    // register functions - only turtle.new
    luaL_newlib(L, PicoTurtle_funcs);

    return 1;
}
