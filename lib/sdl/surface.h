#ifndef QUIPKIT_SURFACE_H_INCLUDED
#define QUIPKIT_SURFACE_H_INCLUDED

#include <lua.h>
#include <SDL.h>

typedef struct {
    SDL_Surface *surface;
    int can_free;   // Whether this surface should be freed.
    int pixel_ref;  // Lua reference to pixel data, if it is userdata. If not, put LUA_NOREF here.
} lua_sdl_surface;

lua_sdl_surface *check_lua_sdl_surface(lua_State *L, int index);
void push_lua_sdl_surface(lua_State *L, SDL_Surface *surface, int can_free, int pixel_ref);
void setup_lua_sdl_surface(lua_State *L, int index);

#endif
