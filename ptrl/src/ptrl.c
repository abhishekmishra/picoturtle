// LUA includes
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdlib.h>
#include <string.h>

#include <raylib.h>

#include "turtle.h"

#include "test_drawing_features.h"

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

    int status;
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

// raylib offscreen gpu rendering
RenderTexture2D init_offscreen_rendering(void)
{
    RenderTexture2D target = LoadRenderTexture(800, 450);
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    EndTextureMode();
    return target;
}

void destroy_offscreen_rendering(RenderTexture2D target)
{
    // Unload the render texture
    UnloadRenderTexture(target);
}

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Lua initialization
    lua_State *L = NULL;
    int res = init_lua(&L);
    if(res != 0)
    {
        printf("Fatal: Unable to initialize Lua!\n");
        return EXIT_FAILURE;
    }

    // Run a Lua script
    const char *script = "print('Hello from Lua!')";
    res = run_lua_script(L, script);
    if (res != LUA_OK)
    {
        printf("Fatal: Unable to run Lua script!\n");
        cleanup_lua(L);
        return EXIT_FAILURE;
    }

    // Initialize Turtle
    trtl_t *turtle = NULL;
    make_turtle(&turtle, "picoturtle", "picoturtle-1");
    if (turtle == NULL)
    {
        printf("Fatal: Unable to create turtle!\n");
        cleanup_lua(L);
        return EXIT_FAILURE;
    }
    // Set turtle's location to 100,100 and heading to 0
    trtl_location_t *location = trtl_get_location(turtle);
    if (location != NULL)
    {
        location_set_x(location, 100.0f);
        location_set_y(location, 100.0f);
    }
    else
    {
        printf("Fatal: Unable to set turtle's location!\n");
        free_turtle(turtle);
        cleanup_lua(L);
        return EXIT_FAILURE;
    }
    // Set heading to 0 degrees
    trtl_state_set_heading(turtle->current_state, 0.0);

    InitWindow(screenWidth, screenHeight, "PicoTurtle");

    SetTargetFPS(60);

    // Initialize offscreen rendering
    RenderTexture2D offscreenTarget = init_offscreen_rendering();
    if (offscreenTarget.id == 0)
    {
        printf("Fatal: Unable to initialize offscreen rendering!\n");
        free_turtle(turtle);
        cleanup_lua(L);
        CloseWindow();
        return EXIT_FAILURE;
    }

    // Initial turtle drawing
    BeginTextureMode(offscreenTarget); // Start drawing to the offscreen target
        // Draw the turtle
        if (turtle != NULL && turtle->current_state != NULL)
        {
            draw_test_graphics_start(turtle);
            trtl_draw_me(turtle);
            // Draw turtle information
            trtl_draw_info(turtle);
        }
        else
        {
            DrawText("Turtle not initialized", 350, 220, 20, RED);
        }
    EndTextureMode(); // End drawing to the offscreen target

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Turtle Draw Loop (if exists)
        BeginTextureMode(offscreenTarget); // Start drawing to the offscreen target
            // Draw the turtle
            if (turtle != NULL && turtle->current_state != NULL)
            {
                // TODO: turtle draw loop (if exists)
            }
            else
            {
                DrawText("Turtle not initialized", 350, 220, 20, RED);
            }
        EndTextureMode(); // End drawing to the offscreen target

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            // Draw the offscreen texture to the screen
            DrawTextureRec(offscreenTarget.texture, (Rectangle){0, 0, offscreenTarget.texture.width, -offscreenTarget.texture.height}, (Vector2){0, 0}, WHITE);            
        EndDrawing();
    }

    // Deallocate the turtle
    if (turtle != NULL)
    {
        free_turtle(turtle);
    }
    else
    {
        printf("Warning: Turtle was not initialized, nothing to free.\n");
    }

    // Deallocate offscreen rendering
    destroy_offscreen_rendering(offscreenTarget);

    // De-Initialization
    CloseWindow();

    // Cleanup Lua
    cleanup_lua(L);
    return EXIT_SUCCESS;
}