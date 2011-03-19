/**
 * See copyright notice in doc/COPYRIGHT.
 */

#include "varray.h"

#include <GL/gl.h>
#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>

#include "texture.h"



/* Draw the vertex data. */
/* -> */
static int VarrayDraw(lua_State *L) {
    sdlgl_Varray *va = luaL_checkudata(L, 1, "sdlgl.varray");
    if (va->num == 0) {
        return luaL_error(L, "attempt to draw empty sdlgl.varray");
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, va->texture_ref);
    luaL_checkudata(L, -1, "sdlgl.texture");

    /* Call the texture's bind method. */
    lua_getfield(L, -1, "bind");
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);

    /* For now, keep arrays off unless we need them. */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_SHORT, 0, va->vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, va->tex_coords);

    glDrawArrays(GL_QUADS, 0, va->num);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    /* Call the texture's unbind method. */
    lua_getfield(L, -1, "unbind");
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);

    return 0;
}


/* __gc finaliser metamethod for sdlgl.varray userdatum. */
static int VarrayGc(lua_State *L) {
    sdlgl_Varray *va = luaL_checkudata(L, 1, "sdlgl.varray");
    luaL_unref(L, LUA_REGISTRYINDEX, va->texture_ref);
    if (va->vertices) { free(va->vertices); }
    if (va->tex_coords) { free(va->tex_coords); }
    return 0;
}


/* Constructor for sdlgl.varray userdatum. Call like sdlgl.varray:new(sdlgl.texture). */
/* userdatum<sdlgl.texture> -> userdatum<sdlgl.varray> */
static int VarrayNew(lua_State *L) {
    luaL_checkudata(L, 2, "sdlgl.texture");

    sdlgl_Varray *va = (sdlgl_Varray *)lua_newuserdata(L, sizeof(sdlgl_Varray));
    if (va == NULL) {
        return luaL_error(L, "cannot allocate new sdlgl.varray userdatum");
    }
    lua_pushvalue(L, 2);
    va->texture_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    va->num = 0;
    va->vertices = NULL;
    va->tex_coords = NULL;

    luaL_getmetatable(L, "sdlgl.varray");
    lua_setmetatable(L, -2);

    return 1;
}


/* Pack per-vertex data into arrays suitable for vertex array drawing. */
/* table<int> vertices, table<float> tex_coords -> */
static int VarrayPack(lua_State *L) {
    sdlgl_Varray *va = luaL_checkudata(L, 1, "sdlgl.varray");
    luaL_checktype(L, 2, LUA_TTABLE);
    luaL_checktype(L, 3, LUA_TTABLE);

    if (lua_objlen(L, 2) != lua_objlen(L, 3)) {
        return luaL_error(L, "mismatched lengths between vertices and tex coords");
    }

    /* Expect only pairs of quads for both vertices and tex coords. */
    if (lua_objlen(L, 2) % (4 * 2) || lua_objlen(L, 3) % (4 * 2)) {
        return luaL_error(L, "incomplete vertex or tex coord data to pack");
    }

    /* For now, varrays can only be packed once. */
    if (va->num > 0) {
        return luaL_error(L, "varray can only be packed once");
    }

    if (lua_objlen(L, 2) == 0) { return 0; }

    unsigned int n = lua_objlen(L, 2) / 2;
    va->num = n;

    va->vertices = (GLshort *)malloc(n * 2 * sizeof(GLshort));
    if (va->vertices == NULL) {
        va->num = 0;
        return luaL_error(L, "could not allocate memory for vertices");
    }
    va->tex_coords = (GLfloat *)malloc(n * 2 * sizeof(GLfloat));
    if (va->tex_coords == NULL) {
        va->num = 0;
        free(va->vertices);
        va->vertices = NULL;
        return luaL_error(L, "could not allocate memory for tex coords");
    }

    for (unsigned int i = 0; i < n; ++i) {
        lua_rawgeti(L, 2, i * 2 + 1);
        lua_rawgeti(L, 2, i * 2 + 2);
        va->vertices[i * 2] = lua_tointeger(L, -2);
        va->vertices[i * 2 + 1] = lua_tointeger(L, -1);

        lua_rawgeti(L, 3, i * 2 + 1);
        lua_rawgeti(L, 3, i * 2 + 2);
        va->tex_coords[i * 2] = lua_tonumber(L, -2);
        va->tex_coords[i * 2 + 1] = lua_tonumber(L, -1);

        lua_pop(L, 4);
    }

    return 0;
}



static const struct luaL_reg sdlgl_VarrayMethods[] = {
    {"__gc", VarrayGc},
    {"draw", VarrayDraw},
    {"new", VarrayNew},
    {"pack", VarrayPack},
    {NULL, NULL}
};

void luaopen_sdlgl_varray(lua_State *L, int module_index) {
	if (module_index < 0) { module_index = lua_gettop(L) + module_index + 1; }
    lua_pushliteral(L, "varray");
    luaL_newmetatable(L, "sdlgl.varray");
    luaL_register(L, NULL, sdlgl_VarrayMethods);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_rawset(L, module_index);
}
