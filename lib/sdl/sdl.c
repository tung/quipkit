#ifdef __MINGW32__
#   include "sdl.h"
#endif

#include <lua.h>
#include <lauxlib.h>
#include <SDL.h>

#include "event.h"
#include "gl.h"
#include "types.h"
#include "wm.h"
#include "video.h"



/**
 * Exported SDL Functions
 */

/* Not an official SDL function, but helps Lua/SDL binding users. */
/* boolean main_result[, string message] -> ... */
static int Assert(lua_State *L) {
    int b = lua_toboolean(L, 1);

    /* Mimic Lua's assert(). */
    const char *msg = luaL_optstring(L, 2, "SDL assertion failed!");
    if (!b) {
        lua_pushstring(L, msg);
        SDL_Quit();     // This call is why Lua's assert() isn't adequate.
        lua_error(L);
    }

    /* Return all arguments passed in. */
    return lua_gettop(L);
}


/* number milliseconds -> */
static int Delay(lua_State *L) {
    Uint32 ms = luaL_checkint(L, 1);
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
    int l;
    Uint32 flags = 0;
    switch (lua_type(L, 1)) {
        case LUA_TNUMBER:
            flags = lua_tointeger(L, 1);
            break;

        case LUA_TTABLE:
            /* Accumulate SDL_INIT_* flags. */
            l = lua_objlen(L, 1);
            for (int i = 1; i <= l; i++) {
                lua_rawgeti(L, 1, i);
                if (!lua_isnumber(L, -1)) {
                    /* All SDL functions except this and Quit throw errors instead. */
                    lua_pushnil(L);
                    lua_pushfstring(L, "expected SDL_INIT_* flag at arg 1 index %d, got %s", i, lua_typename(L, -1));
                    return 2;
                }
                flags |= lua_tointeger(L, -1);
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



/**
 * Public API
 */

static const luasdl_NameConst32 sdl_init_flags[] = {
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

/* Load base SDL constants into SDL module table at index. */
static void add_sdl_constants(lua_State *L, int index) {
    const luasdl_NameConst32 *p;
    for (p = sdl_init_flags; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->uint);    // Assume Lua integers can hold SDL_INIT_* flags.
        lua_settable(L, index < 0 ? index - 2 : index);
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

int
#ifdef __MINGW32__
DLL_EXPORT
#endif
luaopen_luasdl(lua_State *L) {
    luaL_register(L, "SDL", sdl_functions);
    add_sdl_constants(L, -1);

    /* Add sub-modules. */
    LoadSdlEvent(L, -1);
    LoadSdlGl(L, -1);
    load_sdl_video(L, -1);
    load_sdl_wm(L, -1);

    return 1;
}

#ifdef __MINGW32__
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL;
    (void)fdwReason;
    (void)lpvReserved;
    return TRUE;
}
#endif
