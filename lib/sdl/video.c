#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>

#include "video.h"
#include "name_Uint_pair.h"



/* number width, number height, number bpp, (number flag OR array flags{}) -> */
static int SetVideoMode(lua_State *L) {
    int width = luaL_checkint(L, 1);
    int height = luaL_checkint(L, 2);
    int bpp = luaL_checkint(L, 3);

    Uint32 flags = 0;
    int flags_length;
    switch (lua_type(L, 4)) {
        case LUA_TNUMBER:
            flags = lua_tointeger(L, 4);
            break;

        case LUA_TTABLE:
            flags_length = lua_objlen(L, 4);
            for (int i = 1; i <= flags_length; i++) {
                lua_rawgeti(L, 4, i);
                if (!lua_isnumber(L, -1)) {
                    luaL_error(L, "expected SDL_SetVideoMode flag at arg 4 index %d, got %s", i, lua_typename(L, -1));
                }
                flags |= lua_tointeger(L, -1);
                lua_pop(L, 1);
            }
            break;

        default:
            luaL_error(L, "bad argument #4 to 'SetVideoMode' (SDL_SetVideoMode flag or table expected, got %s)", lua_typename(L, 4));
    }

    SDL_Surface *surface = SDL_SetVideoMode(width, height, bpp, flags);
    if (surface == NULL) {
        luaL_error(L, "SDL_SetVideoMode failed: %s", SDL_GetError());
    }

    /* TODO: Return the screen surface somehow. Unlike most surfaces, it doesn't need freeing: SDL_Quit() does that. */
    return 0;
}



static const name_Uint32_pair sdl_SetVideoMode_constants[] = {
    {"SWSURFACE", SDL_SWSURFACE},
    {"HWSURFACE", SDL_HWSURFACE},
    {"ASYNCBLIT", SDL_ASYNCBLIT},
    {"ANYFORMAT", SDL_ANYFORMAT},
    {"HWPALETTE", SDL_HWPALETTE},
    {"DOUBLEBUF", SDL_DOUBLEBUF},
    {"FULLSCREEN", SDL_FULLSCREEN},
    {"OPENGL", SDL_OPENGL},
    {"OPENGLBLIT", SDL_OPENGLBLIT},
    {"RESIZABLE", SDL_RESIZABLE},
    {"NOFRAME", SDL_NOFRAME},
    {NULL, 0}
};

/* Load SDL video constants into SDL module table at index. */
static void load_sdl_video_constants(lua_State *L, int index) {
    const name_Uint32_pair *p;
    for (p = sdl_SetVideoMode_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->uint);    // Assume Lua numbers can hold SDL video constants.
        lua_settable(L, index < 0 ? index - 2 : index);
    }
}


static const luaL_reg sdl_video_functions[] = {
    {"SetVideoMode", SetVideoMode},
    {NULL, NULL}
};

/* Load SDL video functions into SDL module table at index. */
static void load_sdl_video_functions(lua_State *L, int index) {
    const luaL_Reg *reg;
    for (reg = sdl_video_functions; reg->name != NULL; reg++) {
        lua_pushstring(L, reg->name);
        lua_pushcfunction(L, reg->func);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
}

/* Load video API into SDL module table at index. */
void load_sdl_video(lua_State *L, int index) {
    load_sdl_video_functions(L, index);
    load_sdl_video_constants(L, index);
}
