#include "gl.h"

#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>



/* -> */
static int SwapBuffers(lua_State *L) {
    (void)L;
    SDL_GL_SwapBuffers();
    return 0;
}


/* number SDL_GL_attr, number value -> boolean success */
static int SetAttribute(lua_State *L) {
    SDL_GLattr a;
    int v;
    a = luaL_checkint(L, 1);    /* Get SDL_GLattr constant. */
    v = luaL_checkint(L, 2);
    if (SDL_GL_SetAttribute(a, v) != 0) {
        luaL_error(L, "SDL_GL_SetAttribute failed: %s", SDL_GetError());
    }
    lua_pushboolean(L, 1);
    return 1;
}



typedef struct {
    const char *name;
    SDL_GLattr attr;
} name_SDL_GLattr_pair;

static const name_SDL_GLattr_pair sdl_gl_attributes[] = {
    {"DOUBLEBUFFER", SDL_GL_DOUBLEBUFFER},
    {"RED_SIZE", SDL_GL_RED_SIZE},
    {"GREEN_SIZE", SDL_GL_GREEN_SIZE},
    {"BLUE_SIZE", SDL_GL_BLUE_SIZE},
    /* There are many more of these, but I can't find a list of them. */
    {NULL, 0}
};

/* Add SDL GL constants to SDL.GL submodule table at index. */
static void add_sdl_gl_constants(lua_State *L, int index) {
    const name_SDL_GLattr_pair *p;

    for (p = sdl_gl_attributes; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->attr);    /* I hope Lua integers can hold SDL_GLattrs. */
        lua_settable(L, index < 0 ? index - 2 : index);
    }
}


static const struct luaL_reg sdl_gl_functions[] = {
    {"SetAttribute", SetAttribute},
    {"SwapBuffers", SwapBuffers},
    {NULL, NULL}
};

void load_sdl_gl(lua_State *L, int index) {
    lua_pushliteral(L, "GL");
    lua_newtable(L);
    luaL_register(L, NULL, sdl_gl_functions);

    add_sdl_gl_constants(L, -1);

    lua_settable(L, index < 0 ? index - 2 : index);
}
