#include "wm.h"

#include <lua.h>
#include <lauxlib.h>
#include <SDL.h>



/**
 * Exported SDL WM Functions
 */

/* string title, string icon_name -> */
static int SetCaption(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);
    const char *icon_name = luaL_checkstring(L, 2);
    SDL_WM_SetCaption(title, icon_name);
    return 0;
}



/**
 * Public API
 */

static const luaL_reg m_sdl_wm_functions[] = {
    {"SetCaption", SetCaption},
    {NULL, NULL}
};

/* Load window manager API into SDL module table at index. */
void LoadSdlWm(lua_State *L, int index) {
    lua_pushliteral(L, "WM");
    lua_newtable(L);
    luaL_register(L, NULL, m_sdl_wm_functions);
    lua_settable(L, index < 0 ? index - 2 : index);
}
