#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>

#include "event.h"
#include "gl.h"
#include "wm.h"
#include "video.h"

#include "name_Uint_pair.h"



/* Not an official SDL function, but helps Lua/SDL binding users. */
/* boolean main_result[, string message] -> ... */
static int Assert(lua_State *L) {
    int b;
    const char *msg;
    b = lua_toboolean(L, 1);

    /* Mimic Lua's assert(). */
    msg = luaL_optstring(L, 2, "SDL assertion failed!");
    if (!b) {
        lua_pushstring(L, msg);
        SDL_Quit();     /* This call is why Lua's assert() isn't adequate. */
        lua_error(L);
    }

    /* Return all arguments passed in. */
    return lua_gettop(L);
}


/* number milliseconds -> */
static int Delay(lua_State *L) {
    Uint32 ms;
    ms = luaL_checkint(L, 1);
    SDL_Delay(ms);
    return 0;
}


/* -> number millseconds_since_sdl_init */
static int GetTicks(lua_State *L) {
    lua_pushinteger(L, SDL_GetTicks());
    return 1;
}


/* number flag -> boolean success[, string message]     OR  */
/* array flags{} -> boolean success[, string message]       */
static int Init(lua_State *L) {
    int i;
    int l;
    Uint32 flag;
    Uint32 flags = 0;

    switch (lua_type(L, 1)) {
        case LUA_TNUMBER:
            flags = lua_tointeger(L, 1);
            break;

        case LUA_TTABLE:
            /* Accumulate SDL_INIT_* flags. */
            l = lua_objlen(L, 1);
            for (i = 1; i <= l; i++) {
                lua_rawgeti(L, 1, i);
                if (!lua_isnumber(L, -1)) {
                    /* All SDL functions except this and Quit throw errors instead. */
                    lua_pushnil(L);
                    lua_pushfstring(L, "expected SDL_INIT_* flag at arg 1 index %d, got %s", i, lua_typename(L, -1));
                    return 2;
                }
                flag = lua_tointeger(L, -1);
                flags |= flag;
                lua_pop(L, 1);
            }
            break;

        default:
            lua_pushnil(L);
            lua_pushfstring(L, "bad argument #1 to 'Init' (SDL_INIT_* flag or table expected, got %s)", lua_typename(L, 1));
            return 2;
    }

    if (SDL_Init(flags) != 0) {
        lua_pushnil(L);
        lua_pushfstring(L, "SDL_Init failed: %s", SDL_GetError());
        return 2;
    }

    lua_pushboolean(L, 1);
    return 1;
}


/* -> */
static int Quit(lua_State *L) {
    (void)L;
    SDL_Quit();
    return 0;
}



static const name_Uint32_pair sdl_init_flags[] = {
    {"INIT_TIMER", SDL_INIT_TIMER},
    {"INIT_AUDIO", SDL_INIT_AUDIO},
    {"INIT_VIDEO", SDL_INIT_VIDEO},
    {"INIT_CDROM", SDL_INIT_CDROM},
    {"INIT_JOYSTICK", SDL_INIT_JOYSTICK},
    {"INIT_EVERYTHING", SDL_INIT_EVERYTHING},
    {"INIT_NOPARACHUTE", SDL_INIT_NOPARACHUTE},
    {"INIT_EVENTTHREAD", SDL_INIT_EVENTTHREAD},
    {NULL, 0}
};

static void add_sdl_constants(lua_State *L) {
    const name_Uint32_pair *p;

    /* Add SDL_INIT_* flag constants. */
    for (p = sdl_init_flags; p->name != NULL; p++) {
        /* Assume SDL module table is on top of the stack. */
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->uint);    /* I hope Lua integers can hold SDL_INIT_* flags... */
        lua_settable(L, -3);
    }
}


static const struct luaL_reg sdl_functions[] = {
    {"Assert", Assert},
    {"Delay", Delay},
    {"GetTicks", GetTicks},
    {"Init", Init},
    {"Quit", Quit},
    {NULL, NULL}
};

int luaopen_sdl(lua_State *L) {
    luaL_register(L, "SDL", sdl_functions);
    add_sdl_constants(L);

    /* Add sub-modules. */
    load_sdl_event(L, -1);
    load_sdl_gl(L, -1);
    load_sdl_video(L);
    load_sdl_wm(L);

    return 1;
}
