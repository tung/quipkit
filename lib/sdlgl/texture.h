#ifndef SDLGL_TEXTURE_H_INCLUDED
#define SDLGL_TEXTURE_H_INCLUDED

#include <GL/gl.h>
#include <lua.h>


typedef struct {
    int w, h;
    GLuint texId;
    int texW, texH;
    int texHasAlpha;
} sdlgl_Texture;


/* Adds sdlgl.texture to the sdlgl module table. */
void luaopen_sdlgl_texture(lua_State *L, int module_index);

#endif
