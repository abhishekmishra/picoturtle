extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <zclk.h>
#include <stdio.h>
#include <string.h>
#include <coll_arraylist.h>

#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#define TURTLE_LUA_DIR_ENV_VAR      "TURTLE_LUA_DIR"

#define PTCLI_OPT_IMGFILE_NAME      "output"
#define PTCLI_OPT_IMGFILE_SHORT     "o"
#define PTCLI_OPT_IMGFILE_DEFAULT   "turtle.png"
#define PTCLI_OPT_IMGFILE_DESC      "Image filename for PicoTurtle canvas " \
                                    "output. " \
                                    "Note: the image file is written in "\
                                    "PNG format."
#define PTCLI_ARG_FILE_NAME         "path-to-program"
#define PTCLI_ARG_FILE_DEFAULT      NULL
#define PTCLI_ARG_FILE_DESC         "Turtle lua program to execute"

#define TURTLE_EXPORT_CMD_LEN       8192

using namespace turtle;

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
    {											 /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") && /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)		 /* that produces a string? */
            return 1;							 /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1); /* append a standard traceback */
    return 1;					  /* return the traceback */
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
    lua_insert(L, base);			  /* put it under function and args */
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
    //TODO: need to convert to absolute filename.
    //TODO: should add parent folder of filename?
    add_to_lua_path(L, ".");

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

	int status;
	(*Lptr) = luaL_newstate(); // new lua state
    lua_State *L = *Lptr;
    
	if (L == NULL)
	{
		return EXIT_FAILURE;
	}
	
	luaL_openlibs(L);			// open std libraries
	lua_gc(L, LUA_GCGEN, 0, 0); // gc in generational mode

// 	lua_pushcfunction(L, print);
// 	lua_setglobal(L, "print");

	return EXIT_SUCCESS;
}

void turtle_init_cb(turtle::PicoTurtle* t, void* cb_args)
{
}

void turtle_update_cb(turtle::PicoTurtle* t, void* cb_args)
{
}

void turtle_paint_cb(turtle::PicoTurtle* t, void* cb_args)
{
}

void turtle_destroy_cb(turtle::PicoTurtle* t, void* cb_args)
{
}

void turtle_delay(turtle::PicoTurtle* t, int tm)
{
// 	if (turtle_delay_fn != NULL)
// 	{
// 		turtle_delay_fn(t, tm);
// 	}
}

int init_turtle_lua_binding(lua_State *L)
{
	PicoTurtle::set_init_callback(&turtle_init_cb, NULL);
	PicoTurtle::set_update_callback(&turtle_update_cb, NULL);
	PicoTurtle::set_paint_callback(&turtle_paint_cb, NULL);
	PicoTurtle::set_delay_callback(&turtle_delay);
	PicoTurtle::set_destroy_callback(&turtle_destroy_cb, NULL);

	// picoturtle = require "picoturtle"
	luaL_requiref(L, "picoturtle", luaopen_picoturtle, 1);
	lua_pop(L, 1); /* remove result from previous call */

	// TODO: Set path using optional args
	char* turtleLuaDir = getenv(TURTLE_LUA_DIR_ENV_VAR);
	if (turtleLuaDir == NULL || strlen(turtleLuaDir) == 0)
	{
		// turtle_message("app", "Warning: TURTLE_LUA_DIR_ENV_VAR is not set or empty!\n");
		turtleLuaDir = (char*)"lua";
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

	// create the default turtle as global variable t
	run_lua_script(L, "t = require'picoturtle'.new()");

	return 0;
}

/**
 * @brief Main command handler for the PicoTurtle CLI.
 * 
 * @param cmd The command object received from ZClk
 * @param handler_args any args passed to ZClk (unused)
 * @return zclk_res returns error code indicating whether turtle execution
 *  was a success.
 */
zclk_res ptcli_main(zclk_command* cmd, void* handler_args)
{
    lua_State *L = NULL;
    int res = init_lua(&L);
    if(res == 0)
    {
        res = init_turtle_lua_binding(L);
        if (res == 0)
        {
            zclk_argument *turtle_prog = zclk_command_get_argument(cmd, PTCLI_ARG_FILE_NAME);
            zclk_option *img_file = zclk_command_get_option(cmd, PTCLI_OPT_IMGFILE_NAME);
            if(turtle_prog != NULL && img_file != NULL)
            {
                const char *program_path = zclk_argument_get_val_string(turtle_prog);
                const char *img_filename = zclk_option_get_val_string(img_file);

                if (program_path != NULL)
                {
                    printf("Program to run: %s\n", program_path);
                    res = run_lua_file(L, program_path);
                    if (res != 0)
                    {
                        cleanup_lua(L);
                        printf("Error executing Turtle Lua program.\n");
                        return ZCLK_RES_ERR_UNKNOWN;
                    }

                    char lua_export_cmd[TURTLE_EXPORT_CMD_LEN];
                    lua_export_cmd[0] = '\0';

                    snprintf(lua_export_cmd, TURTLE_EXPORT_CMD_LEN, "t:export_img('%s')", img_filename);
                    printf("Running export command: [%s]\n", lua_export_cmd);
                    res = run_lua_script(L, lua_export_cmd);
                    if (res != 0)
                    {
                        cleanup_lua(L);
                        printf("Error executing Turtle Lua program.\n");
                        return ZCLK_RES_ERR_UNKNOWN;
                    }
                }
            }
            else
            {
                cleanup_lua(L);
                printf("Program path not provided.\n");
                return ZCLK_RES_ERR_UNKNOWN;
            }


            cleanup_lua(L);
            return ZCLK_RES_SUCCESS;
        }
        else
        {
            printf("Error initializing picoturtle lua binding.\n");
            cleanup_lua(L);
            return ZCLK_RES_ERR_UNKNOWN;
        }
    }
    else
    {
        printf("Error: Unable to initialize Lua\n");
        handle_lua_error(L, res);
        return ZCLK_RES_ERR_UNKNOWN;
    }
}

int main(int argc, char* argv[])
{
    zclk_command *cmd = new_zclk_command(argv[0], "ptcli",
                            "PicoTurtle CLI", &ptcli_main);

    zclk_command_string_option(
        cmd,
        PTCLI_OPT_IMGFILE_NAME,
        PTCLI_OPT_IMGFILE_SHORT,
        PTCLI_OPT_IMGFILE_DEFAULT,
        PTCLI_OPT_IMGFILE_DESC
    );

    zclk_command_string_argument(
        cmd,
        PTCLI_ARG_FILE_NAME,
        PTCLI_ARG_FILE_DEFAULT,
        PTCLI_ARG_FILE_DESC,
        1
    );
    
    zclk_command_exec(cmd, NULL, argc, argv);
}
