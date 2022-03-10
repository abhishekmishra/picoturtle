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
void turtleInitCb(turtle::PicoTurtle *t, void *cb_args);
void turtleDestroyCb(turtle::PicoTurtle *t, void *cb_args);
int gui_window();

// https://stackoverflow.com/a/7776146/9483968
// Usage:
//     hexDump(desc, addr, len, perLine);
//         desc:    if non-NULL, printed as a description before hex dump.
//         addr:    the address to start dumping from.
//         len:     the number of bytes to dump.
//         perLine: number of bytes on each output line.
void hexDump(
    const char *desc,
    const void *addr,
    const int len,
    int perLine);

static const char *const usages[] = {
    "picoturtle [options] [[--] <turtle file> <turtle args>]",
    "picoturtle [options]",
    NULL,
};

sk_sp<SkImage> img;

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
    // cleanup the turtle lua binding
    cleanupTurtleLuaBinding(L);

    if (gui != 0)
    {
        gui_window();
    }

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
        turtleLuaDir = "lua";
    }
    char *setPathCodeStr = (char *)calloc(strlen(turtleLuaDir) + 1024, sizeof(char));
    if (setPathCodeStr == NULL)
    {
        printf("Fatal: Unable to alloc string to set load path in lua!\n");
    }

    sprintf(setPathCodeStr, "package.path = '%s/?.lua;?.lua;' .. package.path", turtleLuaDir);
    // printf("Setting path via code -> |%s|\n", setPathCodeStr);

    runLuaScript(L, setPathCodeStr);
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

void turtleInitCb(turtle::PicoTurtle *t, void *cb_args)
{
    printf("PicoTurtle created - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
}

void turtleDestroyCb(turtle::PicoTurtle *t, void *cb_args)
{
    printf("PicoTurtle destroyed - Name: %s, Id: %s\n", t->getName().c_str(), t->getId().c_str());
    img = t->getRasterSurface()->makeImageSnapshot();
    if (img)
    {
        printf("Image [%d x %d].\n", img->width(), img->height());
    }
}

int gui_window()
{
    if (img)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "Failed to initialize the SDL2 library\n";
            return -1;
        }

        SDL_Window *window = SDL_CreateWindow("PicoTurtle",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              img->width(), img->height(),
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

        SkImageInfo info = SkImageInfo::MakeN32Premul(img->width(), img->height());
        std::vector<uint32_t> srcPixels;
        const int rowBytes = img->width() * 4;
        srcPixels.resize(img->height() * rowBytes);
        SkPixmap pixmap(info, (const void *)&srcPixels.front(), rowBytes);
        img->readPixels(pixmap, 0, 0);

        // printf("Image data len = %d.\n", srcPixels.size());

        // hexDump("image raw data", &srcPixels.front(), img->height() * rowBytes, rowBytes);

        Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        int shift = (req_format == STBI_rgb) ? 8 : 0;
        rmask = 0xff000000 >> shift;
        gmask = 0x00ff0000 >> shift;
        bmask = 0x0000ff00 >> shift;
        amask = 0x000000ff >> shift;
#else // little endian, like x86
        rmask = 0x00ff0000;
        gmask = 0x0000ff00;
        bmask = 0x000000ff;
        amask = 0xff000000;
#endif

        int depth = 32;
        int pitch = 4 * img->width();

        SDL_Surface *turtleImageSurface = SDL_CreateRGBSurfaceFrom((void *)&srcPixels.front(), img->width(), img->height(), depth, pitch,
                                                                   rmask, gmask, bmask, amask);

        printf("Turtle image surface loaded!\n");

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

                // SDL_BlitSurface(loadedSurface, NULL, window_surface, NULL);
                SDL_BlitSurface(turtleImageSurface, NULL, window_surface, NULL);
                SDL_UpdateWindowSurface(window);
            }
        }
    }

    return 0;
}

void hexDump(
    const char *desc,
    const void *addr,
    const int len,
    int perLine)
{
    // Silently ignore silly per-line values.

    if (perLine < 4 || perLine > 64)
        perLine = 16;

    int i;
    unsigned char *buff = (unsigned char *)calloc(perLine + 1, sizeof(unsigned char));
    const unsigned char *pc = (const unsigned char *)addr;

    // Output description if given.

    if (desc != NULL)
        printf("%s:\n", desc);

    // Length checks.

    if (len == 0)
    {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0)
    {
        printf("  NEGATIVE LENGTH: %d\n", len);
        return;
    }

    // Process every byte in the data.

    for (i = 0; i < len; i++)
    {
        // Multiple of perLine means new or first line (with line offset).

        if ((i % perLine) == 0)
        {
            // Only print previous-line ASCII buffer for lines beyond first.

            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset of current line.

            printf("  %04x ", i);
        }

        // Now the hex code for the specific character.

        printf(" %02x", pc[i]);

        // And buffer a printable ASCII character for later.

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
            buff[i % perLine] = '.';
        else
            buff[i % perLine] = pc[i];
        buff[(i % perLine) + 1] = '\0';
    }

    // Pad out last line if not exactly perLine characters.

    while ((i % perLine) != 0)
    {
        printf("   ");
        i++;
    }

    // And print the final ASCII buffer.

    printf("  %s\n", buff);
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
