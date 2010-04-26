#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>

#include "video.h"
#include "name_Uint_pair.h"



/* number width, number height, number bpp, (number flag OR array flags{}) -> */
static int SetVideoMode(lua_State *L) {
    int width;
    int height;
    int bpp;
    int i;
    SDL_Surface *surface;
    Uint32 flags = 0;
    int flags_length;
    Uint32 flag;

    width = luaL_checkint(L, 1);
    height = luaL_checkint(L, 2);
    bpp = luaL_checkint(L, 3);

    switch (lua_type(L, 4)) {
        case LUA_TNUMBER:
            flags = lua_tointeger(L, 4);
            break;

        case LUA_TTABLE:
            flags_length = lua_objlen(L, 4);
            for (i = 1; i <= flags_length; i++) {
                lua_rawgeti(L, 4, i);
                if (!lua_isnumber(L, -1)) {
                    luaL_error(L, "expected SDL_SetVideoMode flag at arg 4 index %d, got %s", i, lua_typename(L, -1));
                }
                flag = lua_tointeger(L, -1);    /* I hope Lua can reconstitute these flags. */
                flags |= flag;
                lua_pop(L, 1);
            }
            break;

        default:
            luaL_error(L, "bad argument #4 to 'SetVideoMode' (SDL_SetVideoMode flag or table expected, got %s)", lua_typename(L, 4));
    }

    surface = SDL_SetVideoMode(width, height, bpp, flags);
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

static void load_sdl_video_constants(lua_State *L) {
    /* Assume "SDL" module table is on top of the stack. */
    const name_Uint32_pair *p;
    for (p = sdl_SetVideoMode_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->uint);    /* I hope Lua can hold flags in its numbers properly. */
        lua_settable(L, -3);
    }
}


static const luaL_reg sdl_video_functions[] = {
    {"SetVideoMode", SetVideoMode},
    {NULL, NULL}
};

void load_sdl_video(lua_State *L) {
    /* Assume "SDL" module table is on top of the stack. */
    luaL_register(L, NULL, sdl_video_functions);
    load_sdl_video_constants(L);
}
