#include <GL/gl.h>
#include <GL/glpng.h>
#include <lua.h>
#include <lauxlib.h>

#include <stdio.h>



typedef struct {
    const char *name;
    int integer;
} name_integer_pair;



/* string filename, number mipmap, number transparency, number wrap_texture, number minfilter, number magfilter -> table png_texture */
static int PngBind(lua_State *L) {
    const char *filename;
    int mipmap;
    int trans;
    int wrapst;
    int minfilter;
    int magfilter;
    unsigned int id;
    pngInfo info;

    filename = luaL_checkstring(L, 1);
    mipmap = luaL_checkint(L, 2);
    trans = luaL_checkint(L, 3);
    wrapst = luaL_checkint(L, 4);
    minfilter = luaL_checkint(L, 5);
    magfilter = luaL_checkint(L, 6);

    id = pngBind(filename, mipmap, trans, &info, wrapst, minfilter, magfilter);
    if (id == 0) {
        luaL_error(L, "pngBind failed: could not load file \"%s\"", filename);
        return 0;
    }

    lua_createtable(L, 0, 5);

    lua_pushliteral(L, "id");
    lua_pushinteger(L, id);
    lua_rawset(L, -3);

    lua_pushliteral(L, "width");
    lua_pushinteger(L, info.Width);
    lua_rawset(L, -3);

    lua_pushliteral(L, "height");
    lua_pushinteger(L, info.Height);
    lua_rawset(L, -3);

    lua_pushliteral(L, "depth");
    lua_pushinteger(L, info.Depth);
    lua_rawset(L, -3);

    lua_pushliteral(L, "alpha");
    lua_pushinteger(L, info.Alpha);
    lua_rawset(L, -3);

    return 1;
}



static const name_integer_pair glpng_constants[] = {
    /* Mipmapping parameters. */
    {"NOMIPMAPS" , PNG_NOMIPMAPS},
    {"BUILDMIPMAPS", PNG_BUILDMIPMAPS},
    {"SIMPLEMIPMAPS", PNG_SIMPLEMIPMAPS},

    /* Synonymous to the above. */
    {"NOMIPMAP" , PNG_NOMIPMAP},
    {"BUILDMIPMAP", PNG_BUILDMIPMAP},
    {"SIMPLEMIPMAP", PNG_SIMPLEMIPMAP},

    /* Transparency parameters. */
    {"ALPHA", PNG_ALPHA},
    {"SOLID", PNG_SOLID},
    {"STENCIL", PNG_STENCIL},
    {"BLEND1", PNG_BLEND1},
    {"BLEND2", PNG_BLEND2},
    {"BLEND3", PNG_BLEND3},
    {"BLEND4", PNG_BLEND4},
    {"BLEND5", PNG_BLEND5},
    {"BLEND6", PNG_BLEND6},
    {"BLEND7", PNG_BLEND7},
    {"BLEND8", PNG_BLEND8},

    {NULL, 0}
};

static void add_constants(lua_State *L) {
    const name_integer_pair *p;
    for (p = glpng_constants; p->name != NULL; p++) {
        /* Assume glpng table is on top of the stack. */
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->integer);
        lua_rawset(L, -3);
    }
}


static const struct luaL_reg glpng_functions[] = {
    {"PngBind", PngBind},
    {NULL, NULL}
};

int luaopen_glpng(lua_State *L) {
    luaL_register(L, "glpng", glpng_functions);
    add_constants(L);
    return 1;
}
