#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SkiaTurtleLua.hpp"

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

// see https://stackoverflow.com/a/35609473/9483968
// declaration so we can use this in object_new function
int push_skia_turtle_metatable(lua_State *L);

static int skia_turtle_free(lua_State *L)
{
    delete *static_cast<SkiaTurtle **>(luaL_checkudata(L, 1, LUA_SKIA_TURTLE_OBJECT));
    return 0;
}

static int skia_turtle_new(lua_State *L)
{
    SkiaTurtle *t = new SkiaTurtle();
    *static_cast<SkiaTurtle **>(lua_newuserdata(L, sizeof(SkiaTurtle *))) = t;
    push_skia_turtle_metatable(L);
    lua_setmetatable(L, -2);
    return 1;
}

static SkiaTurtle *skia_turtle_getobj(lua_State *L)
{
    int top = lua_gettop(L);
    SkiaTurtle *t = *static_cast<SkiaTurtle **>(luaL_checkudata(L, top, LUA_SKIA_TURTLE_OBJECT));
    if (t == NULL)
    {
        luaL_typeerror(L, top, LUA_SKIA_TURTLE_OBJECT);
    }
    lua_pop(L, 1);
    return t;
}

static int skia_turtle_getwidth(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    lua_pushinteger(L, t->getWidth());
    return 1;
}

static int skia_turtle_setwidth(lua_State *L)
{
    int width = luaL_checkinteger(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->setWidth(width);
    return 0;
}

static int skia_turtle_getheight(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    lua_pushinteger(L, t->getHeight());
    return 1;
}

static int skia_turtle_setheight(lua_State *L)
{
    int height = luaL_checkinteger(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->setHeight(height);
    return 0;
}

static int skia_turtle_penup(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    t->penup();
    return 0;
}

static int skia_turtle_pendown(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    t->pendown();
    return 0;
}

static int skia_turtle_penwidth(lua_State *L)
{
    float width = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->penwidth(width);
    return 0;
}

static int skia_turtle_pencolour(lua_State *L)
{
    unsigned int b = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);
    unsigned int g = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);
    unsigned int r = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->pencolour(r, g, b);
    return 0;
}

static int skia_turtle_stop(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    t->stop();
    return 0;
}

static int skia_turtle_home(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    t->home();
    return 0;
}

static int skia_turtle_clear(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    t->clear();
    return 0;
}

static int skia_turtle_reset(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    t->reset();
    return 0;
}

static int skia_turtle_forward(lua_State *L)
{
    float len = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->forward(len);
    return 0;
}

static int skia_turtle_back(lua_State *L)
{
    float len = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->back(len);
    return 0;
}

static int skia_turtle_setpos(lua_State *L)
{
    float y = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);
    float x = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->setpos(x, y);
    return 0;
}

static int skia_turtle_getx(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    lua_pushnumber(L, t->getx());
    return 1;
}

static int skia_turtle_gety(lua_State *L)
{
    SkiaTurtle *t = skia_turtle_getobj(L);
    lua_pushnumber(L, t->gety());
    return 1;
}

static int skia_turtle_setx(lua_State *L)
{
    float x = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->setx(x);
    return 0;
}

static int skia_turtle_sety(lua_State *L)
{
    float y = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->sety(y);
    return 0;
}

static int skia_turtle_left(lua_State *L)
{
    float a = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->left(a);
    return 0;
}

static int skia_turtle_right(lua_State *L)
{
    float a = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->right(a);
    return 0;
}

static int skia_turtle_heading(lua_State *L)
{
    float a = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->heading(a);
    return 0;
}

static int skia_turtle_export_img(lua_State *L)
{
    const char *s = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->export_img(s);
    return 0;
}

static int skia_turtle_font(lua_State *L)
{
    unsigned int sz = (unsigned int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    const char *str = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->font(str, sz);
    return 0;
}

static int skia_turtle_filltext(lua_State *L)
{
    const char *s = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->filltext(s);
    return 0;
}

static int skia_turtle_stroketext(lua_State *L)
{
    const char *s = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->stroketext(s);
    return 0;
}

static int skia_turtle_canvas_size(lua_State *L)
{
    int height = (int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    int width = (int)(luaL_checkinteger(L, lua_gettop(L)));
    lua_pop(L, 1);

    SkiaTurtle *t = skia_turtle_getobj(L);

    t->canvas_size(width, height);
    return 0;
}

// Pushes the metatable for Object and creates if it doesnt exist yet
int push_skia_turtle_metatable(lua_State *L)
{
    if (luaL_newmetatable(L, LUA_SKIA_TURTLE_OBJECT))
    {
        static const luaL_Reg functions[] =
            {
                {"new", skia_turtle_new},
                {"__gc", skia_turtle_free},
                {"getwidth", skia_turtle_getwidth},
                {"setwidth", skia_turtle_setwidth},
                {"getheight", skia_turtle_getheight},
                {"setheight", skia_turtle_setheight},
                {"penup", skia_turtle_penup},
                {"pendown", skia_turtle_pendown},
                {"penwidth", skia_turtle_penwidth},
                {"pencolour", skia_turtle_pencolour},
                {"stop", skia_turtle_stop},
                {"home", skia_turtle_home},
                {"clear", skia_turtle_clear},
                {"reset", skia_turtle_reset},
                {"forward", skia_turtle_forward},
                {"back", skia_turtle_back},
                {"setpos", skia_turtle_setpos},
                {"getx", skia_turtle_getx},
                {"gety", skia_turtle_gety},
                {"setx", skia_turtle_setx},
                {"sety", skia_turtle_sety},
                {"right", skia_turtle_right},
                {"left", skia_turtle_left},
                {"heading", skia_turtle_heading},
                {"export_img", skia_turtle_export_img},
                {"font", skia_turtle_font},
                {"filltext", skia_turtle_filltext},
                {"stroketext", skia_turtle_stroketext},
                {"canvas_size", skia_turtle_canvas_size},
                {nullptr, nullptr}};
        luaL_setfuncs(L, functions, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
    }
    return 1;
}

bool handleLuaError(lua_State *luaState, int luaErrorCode)
{
    if (luaErrorCode == LUA_OK)
    {
        return true;
    }
    else
    {
        const char *err_msg = lua_tostring(luaState, -1);
        std::cout << err_msg << std::endl;
        return false;
    }
}

int initTurtleLuaBinding(lua_State **luaState, int argc, char *argv[])
{
    // Create Lua State
    (*luaState) = luaL_newstate();
    lua_State *L = (*luaState);

    if (L == NULL)
    {
        return -1;
    }

    // Add standard libraries to Lua Virtual Machine
    luaL_openlibs(L);

    // Register Object metatable for lua (Create and push it)
    push_skia_turtle_metatable(L);
    lua_setglobal(L, LUA_SKIA_TURTLE_OBJECT);

    runLuaFile(L, "lua/TurtleInit.lua");

    const char* fname = NULL;

    lua_newtable(L);
    if (argc > 1)
    {
        fname = argv[1];
        if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {
                lua_pushstring(L, argv[i]);
                lua_seti(L, -2, i - 1);
            }
        }
        //printf("input lua file - %s\n", fname);
    }
    lua_setglobal(L, "arg");

    if (fname != NULL) {
        runLuaFile(L, fname);
    }

    return 0;
}

int runLuaFile(lua_State *luaState, const char *filename)
{
    if (filename != NULL && strlen(filename) > 0)
    {
        if (handleLuaError(luaState, luaL_dofile(luaState, filename)))
        {
            printf("File execution complete -> %s. \n", filename);
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

int runLuaScript(lua_State *luaState, const char *script)
{
    if (script != NULL && strlen(script) > 0)
    {
        if (handleLuaError(luaState, luaL_dostring(luaState, script)))
        {
            printf("Script execution complete.\n");
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

void cleanupTurtleLuaBinding(lua_State *luaState)
{
    lua_close(luaState);
}
