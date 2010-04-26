#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>

#include "wm.h"



/* string title, string icon_name -> */
static int SetCaption(lua_State *L) {
    const char *title;
    const char *icon_name;
    title = luaL_checkstring(L, 1);
    icon_name = luaL_checkstring(L, 2);
    SDL_WM_SetCaption(title, icon_name);
    return 0;
}



static const luaL_reg sdl_wm_functions[] = {
    {"SetCaption", SetCaption},
    {NULL, NULL}
};

void load_sdl_wm(lua_State *L) {
    /* Assume "SDL" module table is on top of the stack. */
    lua_pushliteral(L, "WM");
    lua_newtable(L);
    luaL_register(L, NULL, sdl_wm_functions);
    lua_settable(L, -3);
}
