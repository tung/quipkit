#ifndef __LUAGL_CONST_H__
#define __LUAGL_CONST_H_

#include "luagl_util.h"

extern const luaglConst luagl_const[];

unsigned int luagl_get_gl_enum(lua_State *L, int index);

#endif
