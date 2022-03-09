#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#include "argparse.h"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

int initTurtleLuaBinding(lua_State **luaState);
int handleTurtleLuaArgs(lua_State *L, int argc, char *argv[]);
int runLuaFile(lua_State *luaState, const char *filename);
int runLuaScript(lua_State *luaState, const char *script);
void cleanupTurtleLuaBinding(lua_State *luaState);
void turtleInitCb(turtle::PicoTurtle *t, void* cb_args);
void turtleDestroyCb(turtle::PicoTurtle *t, void* cb_args);
int gui_window();

static const char *const usages[] = {
    "picoturtle [options] [[--] <turtle file> <turtle args>]",
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
        OPT_BOOLEAN('g', "gui", &gui, "run in gui mode.", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\npicoturtle: A turtle programming environment.", "\nTODO: more info.");
    argc = argparse_parse(&argparse, argc, (const char **)argv);

    turtle::PicoTurtle::set_init_callback(&turtleInitCb, NULL);
    turtle::PicoTurtle::set_destroy_callback(&turtleDestroyCb, NULL);

    // initialize the turtle lua binding with args
    initTurtleLuaBinding(&L);
    handleTurtleLuaArgs(L, argc, argv);

    if (gui != 0)
    {
        gui_window();
    }

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

int initTurtleLuaBinding(lua_State **luaState)
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
    return 0;
}

int handleTurtleLuaArgs(lua_State *L, int argc, char *argv[])
{
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

void turtleInitCb(turtle::PicoTurtle *t, void* cb_args)
{
    printf("PicoTurtle created - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
}

void turtleDestroyCb(turtle::PicoTurtle *t, void* cb_args)
{
    printf("PicoTurtle destroyed - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
}

int gui_window()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("PicoTurtle",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          680, 480,
                                          0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return -1;
    }

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }
    else
    {
        std::string path = "out/fern.png";

        // if (!image)
        // {
        //     std::cout << "Failed to load image\n";
        //     std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        //     return -1;
        // }

        SDL_Surface *loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL)
        {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
                    return 1;
        }

        bool keep_window_open = true;
        while (keep_window_open)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e) > 0)
            {
                switch (e.type)
                {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
                }

                SDL_BlitSurface(loadedSurface, NULL, window_surface, NULL);
                SDL_UpdateWindowSurface(window);
            }
        }
    }
    return 0;
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