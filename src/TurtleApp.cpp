#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#include "argparse.h"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

int initTurtleLuaBinding(lua_State **luaState, int argc, char *argv[]);
int runLuaFile(lua_State *luaState, const char *filename);
int runLuaScript(lua_State *luaState, const char *script);
void cleanupTurtleLuaBinding(lua_State *luaState);
void turtleInitCb(Turtle *t);

static const char *const usages[] = {
    "picoturtle [options] [[--] args]",
    "picoturtle [options]",
    NULL,
};

int main(int argc, char *argv[])
{
    // lua state object (see lua api docs)
    lua_State *L;

    // option to indicate if app is to run in gui mode
    int gui = 0;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("PicoTurtle Options"),
        OPT_BOOLEAN('g', "gui", &gui, "force to do", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\npicoturtle: A turtle programming environment.", "\nTODO: more info.");
    argc = argparse_parse(&argparse, argc, (const char**)argv);
    if (gui != 0){
        printf("Not implemented, launch GUI... %d\n", gui);
    }
    // if (argc != 0) {
    //     printf("argc: %d\n", argc);
    //     int i;
    //     for (i = 0; i < argc; i++) {
    //         printf("argv[%d]: %s\n", i, *(argv + i));
    //     }
    // }

    turtle::PicoTurtle::set_init_callback(&turtleInitCb);

    // initialize the turtle lua binding with args
    initTurtleLuaBinding(&L, argc, argv);

    // cleanup the turtle lua binding
    cleanupTurtleLuaBinding(L);

    return 0;
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

    // picoturtle = require "picoturtle"
    luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
    lua_pop(L, 1); /* remove result from previous call */

    // TODO: Set path using optional args
    char *turtleLuaDir = getenv(TURTLE_LUA_DIR_ENV_VAR);
    if (turtleLuaDir == NULL || strlen(turtleLuaDir) == 0)
    {
        printf("Warning: %s is not set or empty!\n", TURTLE_LUA_DIR_ENV_VAR);
    }
    else
    {
        char *setPathCodeStr = (char *)calloc(strlen(turtleLuaDir) + 1024, sizeof(char));
        if (setPathCodeStr == NULL)
        {
            printf("Fatal: Unable to alloc string to set load path in lua!\n");
        }

        sprintf(setPathCodeStr, "package.path = '%s/?.lua;' .. package.path", turtleLuaDir);
        // printf("Setting path via code -> |%s|\n", setPathCodeStr);

        runLuaScript(L, setPathCodeStr);
    }

    // run the TurtleInit file
    // TODO: removed global function bindings, completely remove later
    // runLuaScript(L, "require 'TurtleInit'");

    const char *fname = NULL;

    lua_newtable(L);
    if (argc > 0)
    {
        fname = argv[0];
        if (argc > 1)
        {
            for (int i = 1; i < argc; i++)
            {
                lua_pushstring(L, argv[i]);
                lua_seti(L, -2, i - 1);
            }
        }
        printf("input lua file - %s\n", fname);
    }
    lua_setglobal(L, "arg");

    if (fname != NULL)
    {
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
            // uncomment for debug only.
            // printf("Script execution complete.\n");
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

void turtleInitCb(Turtle *t)
{
    printf("PicoTurtle created - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
}

// void turtle_example()
// {
// turtle::PicoTurtle *skTurtle = new turtle::PicoTurtle();

// skTurtle->pendown();
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->forward(100);
// skTurtle->right(90);
// skTurtle->stop();

// skTurtle->export_img("out/out_raster.png");

// delete skTurtle;
// }