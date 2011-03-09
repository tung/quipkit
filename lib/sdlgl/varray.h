#ifndef SDLGL_VARRAY_H_INCLUDED
#define SDLGL_VARRAY_H_INCLUDED

#include <GL/gl.h>
#include <lua.h>


typedef struct {
    int texture_ref;
    unsigned int num;
    GLshort *vertices;      /* x, y pairs */
    GLfloat *tex_coords;    /* s, t pairs */
} sdlgl_Varray;


/* Adds sdlgl.varray to the sdlgl module table. */
void luaopen_sdlgl_varray(lua_State *L, int module_index);

#endif
