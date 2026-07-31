// LUA includes
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <raylib.h>

#include "turtle.h"
#include "picoturtle_lua.h"
#include "runtime.h"

#define PICOTURTLE_LUA_DIR_ENV_VAR "PICOTURTLE_LUA_DIR"

typedef struct {
    const char *program_path;
    const char *output_path;
    bool show_help;
    bool show_version;
} picoturtle_cli_options_t;

static void print_version(void)
{
    printf(
        "PicoTurtle %s (%s)\n",
        PICOTURTLE_VERSION,
        PICOTURTLE_CODENAME
    );
}

static void print_usage(const char *program_name)
{
    printf(
        "Usage: %s [OPTIONS] <path-to-program>\n\n"
        "PicoTurtle %s (%s)\n\n"
        "Options:\n"
        "  -h, --help           Print this help.\n"
        "  -v, --version        Print version information.\n"
        "  -o, --output PATH    Export the completed canvas to PNG.\n",
        program_name,
        PICOTURTLE_VERSION,
        PICOTURTLE_CODENAME
    );
}

static bool parse_cli_options(
    int argc,
    char **argv,
    picoturtle_cli_options_t *options
) {
    options->program_path = NULL;
    options->output_path = NULL;
    options->show_help = false;
    options->show_version = false;

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            options->show_help = true;
            return true;
        }

        if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            options->show_version = true;
            return true;
        }

        if (strcmp(arg, "-o") == 0 || strcmp(arg, "--output") == 0) {
            if (++i >= argc) {
                fprintf(stderr, "Missing path after %s.\n", arg);
                return false;
            }
            options->output_path = argv[i];
            continue;
        }

        if (strncmp(arg, "--output=", 9) == 0) {
            if (arg[9] == '\0') {
                fprintf(stderr, "Missing path after --output=.\n");
                return false;
            }
            options->output_path = arg + 9;
            continue;
        }

        if (arg[0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", arg);
            return false;
        }

        if (options->program_path != NULL) {
            fprintf(stderr, "Only one Lua program path may be supplied.\n");
            return false;
        }
        options->program_path = arg;
    }

    if (options->program_path == NULL) {
        fprintf(stderr, "A Lua program path is required.\n");
        return false;
    }
    return true;
}

/**
 * accept a lua error code, and print
 * an appropriate message to the console.
 *
 * @param L lua state
 * @param luaErrorCode lua error code
 * @return flag indicating error or not
 */
bool handle_lua_error(lua_State* L, int luaErrorCode)
{
    if (luaErrorCode == LUA_OK)
    {
        return true;
    }
    else
    {
        const char* err_msg = lua_tostring(L, -1);
        printf("%s\n", err_msg);
        return false;
    }
}

/*
** Message handler used to run all chunks
*/
int msghandler(lua_State* L)
{
    const char* msg = lua_tostring(L, 1);
    if (msg == NULL)
    {                                               /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&    /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)         /* that produces a string? */
            return 1;                               /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;                      /* return the traceback */
}

/*
** Interface to 'lua_pcall', which sets appropriate message function
** and C-signal handler. Used to run all chunks.
*
* @param L lua state
* @param narg num arguments
* @param nres num or results
* @return lua error code
*/
int docall(lua_State* L, int narg, int nres)
{
    int status;
    int base = lua_gettop(L) - narg;  /* function index */
    lua_pushcfunction(L, msghandler); /* push message handler */
    lua_insert(L, base);              /* put it under function and args */
    status = lua_pcall(L, narg, nres, base);
    lua_remove(L, base); /* remove message handler from the stack */
    return status;
}

/**
 * run a lua chunk, with the given compilation status
 *
 * @param L lua state
 * @param status
 * @return result status
 */
int dochunk(lua_State* L, int status)
{
    if (status == LUA_OK)
    {
        status = docall(L, 0, 0);
    }
    handle_lua_error(L, status);
    return status;
}

// TODO: add optional script name argument, which is hardcoded
// to "buffer" for now
/**
 * run a lua script as a chunk in the current lua repl
 * @param script
 * @return status
 */
int run_lua_script(lua_State* L, const char* script)
{
    return dochunk(L, luaL_loadbuffer(L, script, strlen(script), "buffer"));
}

/**
 * prepend the given lua path fragment to the lua path variable.
 *
 * the resultant path will be
 *
 * package.path = "<path_fragment>/?.lua;" .. package.path
 *
 * @param L lua state
 * @param path_fragment
 */
void add_to_lua_path(lua_State* L, const char* path_fragment)
{
    size_t len_of_path_str = strlen(path_fragment) + 1024;
    char* setPathCodeStr = (char*)calloc(len_of_path_str, sizeof(char));
    if (setPathCodeStr == NULL)
    {
        printf("Fatal: Unable to alloc string to set load path in lua!\n");
    }
    else
    {
        snprintf(setPathCodeStr, len_of_path_str, "package.path = '%s/?.lua;' .. package.path", path_fragment);
        // for debug
        // print_to_repl("Setting path via code -> |" + std::string(setPathCodeStr) + "|");
        run_lua_script(L, setPathCodeStr);
        free(setPathCodeStr);
    }
}

/**
 * run a lua file as a chunk in the current lua repl
 *
 * @param L lua state
 * @param filename
 * @return status
 */
int run_lua_file(lua_State* L, const char* filename)
{
    // add the current directory to the lua path
    add_to_lua_path(L, ".");

    // now load the script, run it and return the result
    return dochunk(L, luaL_loadfile(L, filename));
}


void cleanup_lua(lua_State *L)
{
    if (L != NULL)
    {
        lua_close(L);
        L = NULL;
    }
}

// TODO: proper message about failure
// trigger error signal if failure.
int init_lua(lua_State ** Lptr)
{
    // cleanup lua if already initialized
    cleanup_lua(*Lptr);

    (*Lptr) = luaL_newstate(); // new lua state
    lua_State *L = *Lptr;

    if (L == NULL)
    {
        return EXIT_FAILURE;
    }

    luaL_openlibs(L);            // open std libraries
    lua_gc(L, LUA_GCGEN, 0, 0); // gc in generational mode

//     lua_pushcfunction(L, print);
//     lua_setglobal(L, "print");

    return EXIT_SUCCESS;
}

int init_turtle_lua_binding(lua_State *L)
{
    // PicoTurtle::set_init_callback(&turtle_init_cb, NULL);
    // PicoTurtle::set_update_callback(&turtle_update_cb, NULL);
    // PicoTurtle::set_paint_callback(&turtle_paint_cb, NULL);
    // PicoTurtle::set_delay_callback(&turtle_delay);
    // PicoTurtle::set_destroy_callback(&turtle_destroy_cb, NULL);

    // picoturtle = require "picoturtle"
    luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
    lua_pop(L, 1); /* remove result from previous call */

    // TODO: Set path using optional args
    char lua_dir_buffer[1024];
    const char *turtleLuaDir = getenv(PICOTURTLE_LUA_DIR_ENV_VAR);
    if (turtleLuaDir == NULL || turtleLuaDir[0] == '\0') {
        if (DirectoryExists("lua")) {
            turtleLuaDir = "lua";
        } else {
            const char *application_dir = GetApplicationDirectory();
            const char *patterns[] = {
                "%s../share/picoturtle/lua",
                "%s../lua"
            };
            turtleLuaDir = "lua";
            for (size_t index = 0;
                 index < sizeof(patterns) / sizeof(patterns[0]); index++) {
                int written = snprintf(
                    lua_dir_buffer,
                    sizeof(lua_dir_buffer),
                    patterns[index],
                    application_dir
                );
                if (written > 0 &&
                    (size_t)written < sizeof(lua_dir_buffer) &&
                    DirectoryExists(lua_dir_buffer)) {
                    turtleLuaDir = lua_dir_buffer;
                    break;
                }
            }
        }
    }

    size_t len_of_path_str = strlen(turtleLuaDir) + 1024;
    char* setPathCodeStr = (char*)calloc(len_of_path_str, sizeof(char));
    if (setPathCodeStr == NULL)
    {
        printf("Fatal: Unable to alloc string to set load path in lua!\n");
        return -2;
    }

    snprintf(setPathCodeStr, len_of_path_str, "package.path = '%s/?.lua;?.lua;' .. package.path", turtleLuaDir);
    // for debug
    // turtle_message("app", QString("Setting path via code -> |") + setPathCodeStr + "|");

    run_lua_script(L, setPathCodeStr);
    free(setPathCodeStr);

    // create the default turtle as global variable t
    run_lua_script(L, "t = require'picoturtle'.new()");
    run_lua_script(L, "print('PicoTurtle Lua binding initialized successfully!')");

    return 0;
}


int picoturtle_main(const char *program_path, const char *output_path)
{
    picoturtle_runtime_t runtime;
    if (!picoturtle_runtime_init(
            &runtime,
            PICOTURTLE_DEFAULT_CANVAS_WIDTH,
            PICOTURTLE_DEFAULT_CANVAS_HEIGHT,
            "PicoTurtle"
        ))
    {
        printf("Error: Unable to initialize the PicoTurtle window and canvas.\n");
        return EXIT_FAILURE;
    }

    lua_State *L = NULL;
    int res = init_lua(&L);
    if(res == 0)
    {
        res = init_turtle_lua_binding(L);
        if (res == 0)
        {
            printf("Program to run: %s\n", program_path);
            res = run_lua_file(L, program_path);
            if (res != 0)
            {
                cleanup_lua(L);
                picoturtle_runtime_destroy(&runtime);
                printf("Error executing Turtle Lua program.\n");
                return EXIT_FAILURE;
            }

            if (output_path != NULL &&
                !picoturtle_runtime_export_png(&runtime, output_path))
            {
                cleanup_lua(L);
                picoturtle_runtime_destroy(&runtime);
                printf("Error exporting the canvas to %s.\n", output_path);
                return EXIT_FAILURE;
            }

            while (!picoturtle_runtime_should_close(&runtime))
            {
                picoturtle_runtime_present(&runtime);
            }
            cleanup_lua(L);
            picoturtle_runtime_destroy(&runtime);
            return EXIT_SUCCESS;
        }
        else
        {
            printf("Error initializing picoturtle lua binding.\n");
            cleanup_lua(L);
            picoturtle_runtime_destroy(&runtime);
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Error: Unable to initialize Lua\n");
        handle_lua_error(L, res);
        picoturtle_runtime_destroy(&runtime);
        return EXIT_FAILURE;
    }
}

int main(int argc, char* argv[])
{
    picoturtle_cli_options_t options;
    if (!parse_cli_options(argc, argv, &options)) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (options.show_help) {
        print_usage(argv[0]);
        return EXIT_SUCCESS;
    }
    if (options.show_version) {
        print_version();
        return EXIT_SUCCESS;
    }

    return picoturtle_main(options.program_path, options.output_path);
}
