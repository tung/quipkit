#include "surface.h"

#include <lua.h>
#include <lauxlib.h>
#include <SDL.h>



/**
 * Declarations
 */

static const char *LUA_SDL_SURFACE_KEY = "Quipkit.SDL.SDL_Surface";



/**
 * Lua SDL Surface Functions
 */

/* Free the lua_sdl_surface struct once it's GC'd. */
static int luasdl_SurfaceFinalize(lua_State *L) {
    luasdl_Surface *s = luasdl_CheckSurface(L, 1);
    if (s->can_free) {
        SDL_FreeSurface(s->surface);
        s->surface = NULL;
        s->can_free = 0;
        luaL_unref(L, LUA_REGISTRYINDEX, s->pixel_ref);
    }
    return 0;
}



/**
 * Exported SDL Functions
 */

/* userdatum<Lua SDL Surface> src, table srcrect{number x, number y, number w, number h}, userdatum<Lua SDL Surface> dst, table dstrect{number x, number y} -> number success*/
static int BlitSurface(lua_State *L) {
#   define EXTRACT_TABLE_INT(arg, member, dest) \
        do { \
            lua_pushliteral(L, #member); \
            lua_gettable(L, arg); \
            if (!lua_isnumber(L, -1)) { \
                return luaL_error(L, "bad argument to #" #arg " to 'BlitSurface' (" #dest "." #member " number expected, got %s)", lua_typename(L, -1)); \
            } \
            dest.member = lua_tointeger(L, -1); \
            lua_pop(L, 1); \
        } while (0)

    luasdl_Surface *src = luasdl_CheckSurface(L, 1);

    /* Get srcrect. */
    SDL_Rect srcrect;
    SDL_Rect *srcrect_final = NULL;
    if (lua_istable(L, 2)) {
        srcrect_final = &srcrect;
        EXTRACT_TABLE_INT(2, x, srcrect);
        EXTRACT_TABLE_INT(2, y, srcrect);
        EXTRACT_TABLE_INT(2, w, srcrect);
        EXTRACT_TABLE_INT(2, h, srcrect);
    } else if (!lua_isnil(L, 2)) {
        return luaL_error(L, "bad argument #2 to 'BlitSurface' (table or nil expected, got %s)", lua_typename(L, 2));
    }

    luasdl_Surface *dst = luasdl_CheckSurface(L, 3);

    /* Get dstrect. */
    SDL_Rect dstrect;
    SDL_Rect *dstrect_final = NULL;
    if (lua_istable(L, 4)) {
        dstrect_final = &dstrect;
        EXTRACT_TABLE_INT(4, x, dstrect);
        EXTRACT_TABLE_INT(4, y, dstrect);
        /* w and h are ignored in dstrect by SDL_BlitSurface, so skip them. */
    } else if (!lua_isnil(L, 4)) {
        return luaL_error(L, "bad argument #4 to 'BlitSurface' (table or nil expected, got %s)", lua_typename(L, 2));
    }

    int result = SDL_BlitSurface(src->surface, srcrect_final, dst->surface, dstrect_final);
    lua_pushinteger(L, result);
    return 1;
#   undef EXTRACT_TABLE_INT
}


/* userdatum pixels, number width, number height, number depth, number pitch, number Rmask, number Gmask, number Bmask, number Amask -> userdatum<Lua SDL Surface> surface */
static int CreateRGBSurfaceFrom(lua_State *L) {
    void *pixels = lua_touserdata(L, 1);
    if (!pixels) {
        return luaL_error(L, "bad argument #1 to 'CreateRGBSurfaceFrom' (userdatum expected, got %s)", lua_typename(L, 1));
    }
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    int depth = luaL_checkinteger(L, 4);
    int pitch = luaL_checkinteger(L, 5);
    Uint32 Rmask = luaL_checkint(L, 6);
    Uint32 Gmask = luaL_checkint(L, 7);
    Uint32 Bmask = luaL_checkint(L, 8);
    Uint32 Amask = luaL_checkint(L, 9);
    SDL_Surface *s = SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch,
                                              Rmask, Gmask, Bmask, Amask);
    if (!s) {
        return luaL_error(L, "SDL_CreateRGBSurfaceFrom failed: %s", SDL_GetError());
    }
    /* Create the surface with a ref to the pixel userdatum, so it lives as long as the surface. */
    luasdl_PushSurface(L, s, 1, luaL_ref(L, LUA_REGISTRYINDEX));
    return 1;
}


/* string file -> userdatum<Lua SDL Surface> bmp */
static int LoadBMP(lua_State *L) {
    const char *file = luaL_checkstring(L, 1);
    SDL_Surface *bmp = SDL_LoadBMP(file);
    if (bmp == NULL) {
        return luaL_error(L, "SDL_LoadBMP failed: %s", SDL_GetError());
    }
    luasdl_PushSurface(L, bmp, 1, LUA_NOREF);
    return 1;
}


/* userdatum<Lua SDL Surface> -> */
static int FreeSurface(lua_State *L) {
    return luasdl_SurfaceFinalize(L);
}



/**
 * Public API
 */

static const luaL_reg m_sdl_surface_functions[] = {
    {"BlitSurface", BlitSurface},
    {"CreateRGBSurfaceFrom", CreateRGBSurfaceFrom},
    {"FreeSurface", FreeSurface},
    {"LoadBMP", LoadBMP},
    {NULL, NULL}
};

/* Load SDL surface functions into SDL module table at index. */
static void LoadSdlSurfaceFunctions(lua_State *L, int index) {
    const luaL_Reg *reg;
    for (reg = m_sdl_surface_functions; reg->name != NULL; reg++) {
        lua_pushstring(L, reg->name);
        lua_pushcfunction(L, reg->func);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
}


/* Check if the element at index is a Lua SDL Surface and if so return it. */
luasdl_Surface *luasdl_CheckSurface(lua_State *L, int index) {
    void *ud = luaL_checkudata(L, index, LUA_SDL_SURFACE_KEY);
    luaL_argcheck(L, ud != NULL, index, "SDL Surface expected");
    return (luasdl_Surface *)ud;
}


/* Push a new Lua SDL Surface userdatum onto the stack. */
void luasdl_PushSurface(lua_State *L, SDL_Surface *surface, int can_free, int pixel_ref) {
    luasdl_Surface *s = (luasdl_Surface *)lua_newuserdata(L, sizeof(luasdl_Surface));
    luaL_getmetatable(L, LUA_SDL_SURFACE_KEY);
    lua_setmetatable(L, -2);

    s->surface = surface;
    s->can_free = can_free;
    s->pixel_ref = pixel_ref;
}


/* Setup Lua SDL Surface userdata with the SDL module table at index. */
void luasdl_SetupSurface(lua_State *L, int index) {
    LoadSdlSurfaceFunctions(L, index);

    /* Prepare GC finalizer needed for most, but not all, surfaces. */
    luaL_newmetatable(L, LUA_SDL_SURFACE_KEY);
    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, luasdl_SurfaceFinalize);
    lua_rawset(L, -3);
    lua_pop(L, 1);
}
