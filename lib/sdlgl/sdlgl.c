/**
 * See copyright notice in doc/COPYRIGHT.
 */

#ifdef _WIN32
#   include "sdlgl.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <lua.h>
#include <lauxlib.h>
#include <SDL.h>

#include "texture.h"
#include "varray.h"



static int CheckGlError(lua_State *L) {
    GLenum gl_error = glGetError();
    if (gl_error != GL_NO_ERROR)
        return luaL_error(L, "OpenGL error: %s", gluErrorString(gl_error));
    return 0;
}


int
#ifdef _WIN32
DLL_EXPORT
#endif
luaopen_sdlgl(lua_State *L) {
    lua_createtable(L, 0, 3);

    luaopen_sdlgl_texture(L, -1);
    luaopen_sdlgl_varray(L, -1);

    lua_pushliteral(L, "checkGlError");
    lua_pushcfunction(L, CheckGlError);
    lua_rawset(L, -3);

    lua_setglobal(L, "sdlgl");
    lua_getglobal(L, "sdlgl");
    return 1;
}

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL;
    (void)fdwReason;
    (void)lpvReserved;
    return TRUE;
}
#endif
