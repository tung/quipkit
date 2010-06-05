#ifndef QUIPKIT_SDL_SURFACE_H_INCLUDED
#define QUIPKIT_SDL_SURFACE_H_INCLUDED

#include <lua.h>
#include <SDL.h>

typedef struct {
    SDL_Surface *surface;
    int can_free;
} lua_sdl_surface;

lua_sdl_surface *check_lua_sdl_surface(lua_State *L, int index);
void push_lua_sdl_surface(lua_State *L, SDL_Surface *surface, int can_free);
void setup_lua_sdl_surface(lua_State *L, int index);

#endif
