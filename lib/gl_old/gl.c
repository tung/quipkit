#include <GL/gl.h>
#include <lua.h>
#include <lauxlib.h>



static int MatrixMode(lua_State *L) {
    GLenum mode;
    mode = luaL_checkint(L, 1);
    glMatrixMode(mode);
    return 0;
}

static int Viewport(lua_State *L) {
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
    x = luaL_checkint(L, 1);
    y = luaL_checkint(L, 2);
    width = luaL_checkint(L, 3);
    height = luaL_checkint(L, 4);
    glViewport(x, y, width, height);
    return 0;
}



typedef struct {
    const char *name;
    GLenum e;
} name_GLenum_pair;

static const name_GLenum_pair gl_MatrixMode_constants[] = {
    {"MODELVIEW", GL_MODELVIEW},
    {"PROJECTION", GL_PROJECTION},
    {"TEXTURE", GL_TEXTURE},
    {"COLOR", GL_COLOR},
    {NULL, 0}
};

void add_gl_constants(lua_State *L) {
    /* Assume "gl" module table is on top of the stack. */
    const name_GLenum_pair *p;
    for (p = gl_MatrixMode_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->e);   /* Assume Lua numbers can hold gl constants. */
        lua_settable(L, -3);
    }
}


static const luaL_reg gl_functions[] = {
    {"MatrixMode", MatrixMode},
    {"Viewport", Viewport},
    {NULL, NULL}
};

int luaopen_gl(lua_State *L) {
    luaL_register(L, "gl", gl_functions);
    add_gl_constants(L);
    return 1;
}
