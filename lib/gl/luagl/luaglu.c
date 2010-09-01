/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2007(c) Fabio Guerra
*  luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the GLU
*               binding for Lua 5
*-------------------------------------------------
* Changed by Antonio Scuri for LuaForge
*  http://luagl.luaforge.net
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif
#if defined (__APPLE__) || defined (OSX)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <lua.h>
#include <lauxlib.h>

#include "luaglu.h"
#include "luagl_const.h"
#include "luagl_util.h"

/* old headers does not have this */
#ifndef GLU_INCOMPATIBLE_GL_VERSION
#define GLU_INCOMPATIBLE_GL_VERSION     100903
#endif

static const luaglConst luaglu_const[] = {
  {"FALSE"	,GLU_FALSE},
  {"TRUE"	,GLU_TRUE},
  {"VERSION_1_1"	,GLU_VERSION_1_1},
  {"VERSION_1_2"	,GLU_VERSION_1_2},
#ifdef GLU_VERSION_1_3
  {"VERSION_1_3"	,GLU_VERSION_1_3},
#endif
  {"VERSION"	,GLU_VERSION},
  {"EXTENSIONS"	,GLU_EXTENSIONS},
  {"INVALID_ENUM"	,GLU_INVALID_ENUM},
  {"INVALID_VALUE"	,GLU_INVALID_VALUE},
  {"OUT_OF_MEMORY"	,GLU_OUT_OF_MEMORY},
  {"INCOMPATIBLE_GL_VERSION"	,GLU_INCOMPATIBLE_GL_VERSION},
#ifdef GLU_VERSION_1_3
  {"INVALID_OPERATION"	,GLU_INVALID_OPERATION},
#endif
  {"OUTLINE_POLYGON"	,GLU_OUTLINE_POLYGON},
  {"OUTLINE_PATCH"	,GLU_OUTLINE_PATCH},
  {"ERROR"	,GLU_ERROR},
#ifdef GLU_VERSION_1_3
  {"NURBS_ERROR"	,GLU_NURBS_ERROR},
  {"NURBS_BEGIN"	,GLU_NURBS_BEGIN},
  {"NURBS_BEGIN_EXT"	,GLU_NURBS_BEGIN_EXT},
  {"NURBS_VERTEX"	,GLU_NURBS_VERTEX},
  {"NURBS_VERTEX_EXT"	,GLU_NURBS_VERTEX_EXT},
  {"NURBS_NORMAL"	,GLU_NURBS_NORMAL},
  {"NURBS_NORMAL_EXT"	,GLU_NURBS_NORMAL_EXT},
  {"NURBS_COLOR"	,GLU_NURBS_COLOR},
  {"NURBS_COLOR_EXT"	,GLU_NURBS_COLOR_EXT},
  {"NURBS_TEXTURE_COORD"	,GLU_NURBS_TEXTURE_COORD},
  {"NURBS_TEX_COORD_EXT"	,GLU_NURBS_TEX_COORD_EXT},
  {"NURBS_END"	,GLU_NURBS_END},
  {"NURBS_END_EXT"	,GLU_NURBS_END_EXT},
  {"NURBS_BEGIN_DATA"	,GLU_NURBS_BEGIN_DATA},
  {"NURBS_BEGIN_DATA_EXT"	,GLU_NURBS_BEGIN_DATA_EXT},
  {"NURBS_VERTEX_DATA"	,GLU_NURBS_VERTEX_DATA},
  {"NURBS_VERTEX_DATA_EXT"	,GLU_NURBS_VERTEX_DATA_EXT},
  {"NURBS_NORMAL_DATA"	,GLU_NURBS_NORMAL_DATA},
  {"NURBS_NORMAL_DATA_EXT"	,GLU_NURBS_NORMAL_DATA_EXT},
  {"NURBS_COLOR_DATA"	,GLU_NURBS_COLOR_DATA},
  {"NURBS_COLOR_DATA_EXT"	,GLU_NURBS_COLOR_DATA_EXT},
  {"NURBS_TEXTURE_COORD_DATA"	,GLU_NURBS_TEXTURE_COORD_DATA},
  {"NURBS_TEX_COORD_DATA_EXT"	,GLU_NURBS_TEX_COORD_DATA_EXT},
  {"NURBS_END_DATA"	,GLU_NURBS_END_DATA},
  {"NURBS_END_DATA_EXT"	,GLU_NURBS_END_DATA_EXT},
  {"NURBS_MODE"	,GLU_NURBS_MODE},
  {"NURBS_MODE_EXT"	,GLU_NURBS_MODE_EXT},
  {"NURBS_TESSELLATOR"	,GLU_NURBS_TESSELLATOR},
  {"NURBS_TESSELLATOR_EXT"	,GLU_NURBS_TESSELLATOR_EXT},
  {"NURBS_RENDERER"	,GLU_NURBS_RENDERER},
  {"NURBS_RENDERER_EXT"	,GLU_NURBS_RENDERER_EXT},
#endif
  {"NURBS_ERROR1"	,GLU_NURBS_ERROR1},
  {"NURBS_ERROR2"	,GLU_NURBS_ERROR2},
  {"NURBS_ERROR3"	,GLU_NURBS_ERROR3},
  {"NURBS_ERROR4"	,GLU_NURBS_ERROR4},
  {"NURBS_ERROR5"	,GLU_NURBS_ERROR5},
  {"NURBS_ERROR6"	,GLU_NURBS_ERROR6},
  {"NURBS_ERROR7"	,GLU_NURBS_ERROR7},
  {"NURBS_ERROR8"	,GLU_NURBS_ERROR8},
  {"NURBS_ERROR9"	,GLU_NURBS_ERROR9},
  {"NURBS_ERROR10"	,GLU_NURBS_ERROR10},
  {"NURBS_ERROR11"	,GLU_NURBS_ERROR11},
  {"NURBS_ERROR12"	,GLU_NURBS_ERROR12},
  {"NURBS_ERROR13"	,GLU_NURBS_ERROR13},
  {"NURBS_ERROR14"	,GLU_NURBS_ERROR14},
  {"NURBS_ERROR15"	,GLU_NURBS_ERROR15},
  {"NURBS_ERROR16"	,GLU_NURBS_ERROR16},
  {"NURBS_ERROR17"	,GLU_NURBS_ERROR17},
  {"NURBS_ERROR18"	,GLU_NURBS_ERROR18},
  {"NURBS_ERROR19"	,GLU_NURBS_ERROR19},
  {"NURBS_ERROR20"	,GLU_NURBS_ERROR20},
  {"NURBS_ERROR21"	,GLU_NURBS_ERROR21},
  {"NURBS_ERROR22"	,GLU_NURBS_ERROR22},
  {"NURBS_ERROR23"	,GLU_NURBS_ERROR23},
  {"NURBS_ERROR24"	,GLU_NURBS_ERROR24},
  {"NURBS_ERROR25"	,GLU_NURBS_ERROR25},
  {"NURBS_ERROR26"	,GLU_NURBS_ERROR26},
  {"NURBS_ERROR27"	,GLU_NURBS_ERROR27},
  {"NURBS_ERROR28"	,GLU_NURBS_ERROR28},
  {"NURBS_ERROR29"	,GLU_NURBS_ERROR29},
  {"NURBS_ERROR30"	,GLU_NURBS_ERROR30},
  {"NURBS_ERROR31"	,GLU_NURBS_ERROR31},
  {"NURBS_ERROR32"	,GLU_NURBS_ERROR32},
  {"NURBS_ERROR33"	,GLU_NURBS_ERROR33},
  {"NURBS_ERROR34"	,GLU_NURBS_ERROR34},
  {"NURBS_ERROR35"	,GLU_NURBS_ERROR35},
  {"NURBS_ERROR36"	,GLU_NURBS_ERROR36},
  {"NURBS_ERROR37"	,GLU_NURBS_ERROR37},
  {"AUTO_LOAD_MATRIX"	,GLU_AUTO_LOAD_MATRIX},
  {"CULLING"	,GLU_CULLING},
  {"SAMPLING_TOLERANCE"	,GLU_SAMPLING_TOLERANCE},
  {"DISPLAY_MODE"	,GLU_DISPLAY_MODE},
  {"PARAMETRIC_TOLERANCE"	,GLU_PARAMETRIC_TOLERANCE},
  {"SAMPLING_METHOD"	,GLU_SAMPLING_METHOD},
  {"U_STEP"	,GLU_U_STEP},
  {"V_STEP"	,GLU_V_STEP},
#ifdef GLU_VERSION_1_3
  {"OBJECT_PARAMETRIC_ERROR"	,GLU_OBJECT_PARAMETRIC_ERROR},
  {"OBJECT_PARAMETRIC_ERROR_EXT"	,GLU_OBJECT_PARAMETRIC_ERROR_EXT},
  {"OBJECT_PATH_LENGTH"	,GLU_OBJECT_PATH_LENGTH},
  {"OBJECT_PATH_LENGTH_EXT"	,GLU_OBJECT_PATH_LENGTH_EXT},
#endif
  {"PATH_LENGTH"	,GLU_PATH_LENGTH},
  {"PARAMETRIC_ERROR"	,GLU_PARAMETRIC_ERROR},
  {"DOMAIN_DISTANCE"	,GLU_DOMAIN_DISTANCE},
  {"MAP1_TRIM_2"	,GLU_MAP1_TRIM_2},
  {"MAP1_TRIM_3"	,GLU_MAP1_TRIM_3},
  {"POINT"	,GLU_POINT},
  {"LINE"	,GLU_LINE},
  {"FILL"	,GLU_FILL},
  {"SILHOUETTE"	,GLU_SILHOUETTE},
  {"SMOOTH"	,GLU_SMOOTH},
  {"FLAT"	,GLU_FLAT},
  {"NONE"	,GLU_NONE},
  {"OUTSIDE"	,GLU_OUTSIDE},
  {"INSIDE"	,GLU_INSIDE},
  {"TESS_BEGIN"	,GLU_TESS_BEGIN},
  {"BEGIN"	,GLU_BEGIN},
  {"TESS_VERTEX"	,GLU_TESS_VERTEX},
  {"VERTEX"	,GLU_VERTEX},
  {"TESS_END"	,GLU_TESS_END},
  {"END"	,GLU_END},
  {"TESS_ERROR"	,GLU_TESS_ERROR},
  {"TESS_EDGE_FLAG"	,GLU_TESS_EDGE_FLAG},
  {"EDGE_FLAG"	,GLU_EDGE_FLAG},
  {"TESS_COMBINE"	,GLU_TESS_COMBINE},
  {"TESS_BEGIN_DATA"	,GLU_TESS_BEGIN_DATA},
  {"TESS_VERTEX_DATA"	,GLU_TESS_VERTEX_DATA},
  {"TESS_END_DATA"	,GLU_TESS_END_DATA},
  {"TESS_ERROR_DATA"	,GLU_TESS_ERROR_DATA},
  {"TESS_EDGE_FLAG_DATA"	,GLU_TESS_EDGE_FLAG_DATA},
  {"TESS_COMBINE_DATA"	,GLU_TESS_COMBINE_DATA},
  {"CW"	,GLU_CW},
  {"CCW"	,GLU_CCW},
  {"INTERIOR"	,GLU_INTERIOR},
  {"EXTERIOR"	,GLU_EXTERIOR},
  {"UNKNOWN"	,GLU_UNKNOWN},
  {"TESS_WINDING_RULE"	,GLU_TESS_WINDING_RULE},
  {"TESS_BOUNDARY_ONLY"	,GLU_TESS_BOUNDARY_ONLY},
  {"TESS_TOLERANCE"	,GLU_TESS_TOLERANCE},
  {"TESS_ERROR1"	,GLU_TESS_ERROR1},
  {"TESS_ERROR2"	,GLU_TESS_ERROR2},
  {"TESS_ERROR3"	,GLU_TESS_ERROR3},
  {"TESS_ERROR4"	,GLU_TESS_ERROR4},
  {"TESS_ERROR5"	,GLU_TESS_ERROR5},
  {"TESS_ERROR6"	,GLU_TESS_ERROR6},
  {"TESS_ERROR7"	,GLU_TESS_ERROR7},
  {"TESS_ERROR8"	,GLU_TESS_ERROR8},
  {"TESS_MISSING_BEGIN_POLYGON"	,GLU_TESS_MISSING_BEGIN_POLYGON},
  {"TESS_MISSING_BEGIN_CONTOUR"	,GLU_TESS_MISSING_BEGIN_CONTOUR},
  {"TESS_MISSING_END_POLYGON"	,GLU_TESS_MISSING_END_POLYGON},
  {"TESS_MISSING_END_CONTOUR"	,GLU_TESS_MISSING_END_CONTOUR},
  {"TESS_COORD_TOO_LARGE"	,GLU_TESS_COORD_TOO_LARGE},
  {"TESS_NEED_COMBINE_CALLBACK"	,GLU_TESS_NEED_COMBINE_CALLBACK},
  {"TESS_WINDING_ODD"	,GLU_TESS_WINDING_ODD},
  {"TESS_WINDING_NONZERO"	,GLU_TESS_WINDING_NONZERO},
  {"TESS_WINDING_POSITIVE"	,GLU_TESS_WINDING_POSITIVE},
  {"TESS_WINDING_NEGATIVE"	,GLU_TESS_WINDING_NEGATIVE},
  {"TESS_WINDING_ABS_GEQ_TWO"	,GLU_TESS_WINDING_ABS_GEQ_TWO},
  {0, 0}
};

static GLenum luaglu_get_gl_enum(lua_State *L, int index)
{
  return luagl_get_enum(L, index, luaglu_const);
}

/* GetString (name) -> string */
static int luaglu_get_string(lua_State *L)
{
  lua_pushstring(L, (char*)gluGetString(luaglu_get_gl_enum(L, 1)));
  return 1;
}

/* Ortho2D(left, right, bottom, top) -> none */
static int luaglu_ortho_2D(lua_State *L)
{
  gluOrtho2D(luaL_checknumber(L, 1), luaL_checknumber(L, 2), 
             luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/* Perspective(fovy, aspect, near, far) -> none */
static int luaglu_perspective(lua_State *L)
{
  gluPerspective(luaL_checknumber(L, 1), luaL_checknumber(L, 2), 
                 luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/* LookAt(Ex, Ey, Ez, Lx, Ly, Lz, Ux, Uy, Uz) -> none */
static int luaglu_look_at(lua_State *L)
{
  gluLookAt(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3),
            luaL_checknumber(L, 4), luaL_checknumber(L, 5), luaL_checknumber(L, 6),
            luaL_checknumber(L, 7), luaL_checknumber(L, 8), luaL_checknumber(L, 9));
  return 0;
}

/* PickMatrix( x, y, deltax, deltay, viewportArray) -> none */
static int luaglu_pick_matrix(lua_State *L)
{
  GLint *vp;
  int n;

  n = luagl_get_arrayi(L, 5, &vp);
  if (n < 4)
    luaL_argerror(L, 5, "invalid number of elements in the matrix table (n<4).");

  gluPickMatrix(luaL_checknumber(L, 1), luaL_checknumber(L, 2), 
                luaL_checknumber(L, 3), luaL_checknumber(L, 4), vp);

  LUAGL_DELETE_ARRAY(vp);

  return 0;
}

/* ErrorString(errorCode) -> string */
static int luaglu_error_string(lua_State *L)
{
  lua_pushstring(L, (char *)gluErrorString((GLenum)luaL_checkinteger(L, 1)));
  return 1;
}

/*********************************** GLU Mipmapping ***********************************/

/*int gluScaleImage( GLenum format, GLsizei widthin,
GLsizei heightin, GLenum typein, const void *datain,
GLsizei widthout, GLsizei heightout, GLenum typeout,
void *dataout );*/
/* not implemented */

/*int gluBuild1DMipmaps( GLenum target,
GLint internalFormat, GLsizei width, GLenum format,
GLenum type, const void *data );*/
/* not implemented */

/*int gluBuild2DMipmaps( GLenum target,
GLint internalFormat, GLsizei width, GLsizei height,
GLenum format, GLenum type, const void *data );*/
/* not implemented */

/*Build2DMipmaps(textureData) -> error */
static int luaglu_build_2d_mipmaps(lua_State *L)
{
  GLenum target, format, type;
  GLubyte *pixels;
  GLint internalFormat;
  GLsizei width, height, w, h;
  int result;

  h = luagl_get_array2uc(L, 1, &pixels, &w);

  lua_getfield(L, 1, "target");  target = luagl_get_gl_enum(L, -1);  lua_remove(L, -1);
  lua_getfield(L, 1, "format");  format = luagl_get_gl_enum(L, -1);  lua_remove(L, -1);
  lua_getfield(L, 1, "type");    type   = luagl_get_gl_enum(L, -1);  lua_remove(L, -1);
  lua_getfield(L, 1, "width");   width  = (GLsizei)luaL_checkinteger(L, -1);  lua_remove(L, -1);
  lua_getfield(L, 1, "height");  height = (GLsizei)luaL_checkinteger(L, -1);  lua_remove(L, -1);
  lua_getfield(L, 1, "components");  internalFormat = (GLint)luaL_checkinteger(L, -1);  lua_remove(L, -1);

  w /= internalFormat;

  if (width > w)
    width = w;

  if (height > h)
    height = h;

  result = gluBuild2DMipmaps(target, internalFormat, width, height, format, type, pixels);

  LUAGL_DELETE_ARRAY(pixels);

  lua_pushnumber(L, result);

  return 1;
}

/*int gluBuild3DMipmaps( GLenum target,
GLint internalFormat, GLsizei width, GLsizei height,
GLsizei depth, GLenum format, GLenum type,
const void *data );*/
/* not implemented */


/* GLU Quadrics */

#define LUAGLUQUADRIC "GLUquadric"

/* GLUquadric userdata */

typedef struct LuaGLUquadric{
  GLUquadric* quad;
} LuaGLUquadric;


static LuaGLUquadric * checkLuaGLUquadric(lua_State *L, int index) 
{
  LuaGLUquadric * lquad = (LuaGLUquadric *)luaL_checkudata(L,index,LUAGLUQUADRIC);
  luaL_argcheck(L,lquad != NULL,index,"GLUquadric expected");
  return lquad;
}

static LuaGLUquadric * pushLuaGLUquadric(lua_State *L) 
{
  LuaGLUquadric * lquad = (LuaGLUquadric *)lua_newuserdata(L,sizeof(LuaGLUquadric));
  luaL_getmetatable(L,LUAGLUQUADRIC);
  lua_setmetatable(L,-2);
  return lquad;
}

/* glu.NewQuadric() -> quad */
static int luaglu_new_quadric(lua_State *L) 
{
  LuaGLUquadric *lquad;
  GLUquadric* quad = gluNewQuadric();
  if (!quad) 
    luaL_error(L, "glu.NewQuadric failed");

  lquad = pushLuaGLUquadric(L);
  lquad->quad = quad;
  return 1;
}

static int luaglu_quadric_gc(lua_State *L) 
{
  LuaGLUquadric * lquad=checkLuaGLUquadric(L,1);
  if (lquad->quad) gluDeleteQuadric(lquad->quad);
  return 0;
}

static int luaglu_quadric_tostring(lua_State *L) 
{
  LuaGLUquadric * lquad=checkLuaGLUquadric(L,1);
  lua_pushfstring(L,"GLUquadric <%p>",lquad->quad);
  return 1;
}

/* quad:Cylinder(base, top, height, slices, stacks) -> quad */
static int luaglu_cylinder(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);

  gluCylinder(lquad->quad,luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4),
              luaL_checkinteger(L, 5), luaL_checkinteger(L, 6));

  lua_pushvalue(L,1);
  return 1;
}

/* quad:Disk(inner, outer, slices, loops) -> quad */
static int luaglu_disk(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);

  gluDisk(lquad->quad,luaL_checknumber(L, 2), luaL_checknumber(L, 3),
    luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));

  lua_pushvalue(L,1);
  return 1;
}

/* quad:PartialDisk (inner, outer, slices, loops, start, sweep) -> quad */
static int luaglu_partial_disk(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);

  gluPartialDisk(lquad->quad,luaL_checknumber(L, 2), luaL_checknumber(L, 3), 
		             luaL_checkinteger(L, 4),luaL_checkinteger(L, 5), 
		             luaL_checknumber(L, 6),luaL_checknumber(L, 7));

  lua_pushvalue(L,1);
  return 1;

}

/* void GLAPIENTRY gluQuadricCallback (GLUquadric* quad, GLenum which, _GLUfuncptr CallBackFunc); */
/* not implemented */

/* quad:DrawStyle(draw) -> quad */
static int luaglu_quadric_draw_style(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);
  gluQuadricDrawStyle(lquad->quad, luaglu_get_gl_enum(L, 2));
  lua_pushvalue(L,1);
  return 1;
}

/* quad:Normals(normal) -> quad */
static int luaglu_quadric_normals(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);
  gluQuadricNormals(lquad->quad, luaglu_get_gl_enum(L, 2));
  lua_pushvalue(L,1);
  return 1;

}

/* quad:Orientation(orientation) -> quad */
static int luaglu_quadric_orientation(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);
  gluQuadricOrientation(lquad->quad, luaglu_get_gl_enum(L, 2));
  lua_pushvalue(L,1);
  return 1;

}

/* quad:Texture(texture) -> quad */
static int luaglu_quadric_texture(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);
  gluQuadricTexture(lquad->quad, luagl_checkboolean(L,2));
  lua_pushvalue(L,1);
  return 1;
}

/* quad:Sphere(radius, slices, stacks) -> quad */
static int luaglu_sphere(lua_State *L)
{
  LuaGLUquadric *lquad=checkLuaGLUquadric(L,1);
  gluSphere(lquad->quad,luaL_checknumber(L, 2),luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  lua_pushvalue(L,1);
  return 1;
}

/* quadric related functions are implemented as Quadric userdata methods */
static const luaL_reg luagluquadric_methods[] = {
  {"__gc",luaglu_quadric_gc},
  {"__tostring",luaglu_quadric_tostring},

  {"Disk", luaglu_disk},
  {"PartialDisk", luaglu_partial_disk},
  {"Cylinder", luaglu_cylinder},
  {"Sphere", luaglu_sphere},
  {"DrawStyle", luaglu_quadric_draw_style},
  {"Normals", luaglu_quadric_normals},
  {"Orientation", luaglu_quadric_orientation},
  {"Texture", luaglu_quadric_texture},

  {NULL, NULL}
};


/* GLU Nurbs */

#define LUAGLUNURB "GLUnurb"

/* GLUnurb userdata */
typedef struct LuaGLUnurb{
  GLUnurbs* nurb;
  lua_State * L;
#ifdef GLU_VERSION_1_3
  int ref_cb;
#endif
} LuaGLUnurb;


static LuaGLUnurb* checkLuaGLUnurb(lua_State *L, int index) 
{
  LuaGLUnurb * lnurb = (LuaGLUnurb *)luaL_checkudata(L,index,LUAGLUNURB);
  luaL_argcheck(L,lnurb != NULL,index,"GLUnurb expected");
  return lnurb;
}

static LuaGLUnurb* pushLuaGLUnurb(lua_State *L) 
{
  LuaGLUnurb * lnurb = (LuaGLUnurb *)lua_newuserdata(L,sizeof(LuaGLUnurb));
  luaL_getmetatable(L,LUAGLUNURB);
  lua_setmetatable(L,-2);
  return lnurb;
}

/* glu.NewNurbsRenderer() -> nurb */
static int luaglu_new_nurbs(lua_State *L) 
{
  LuaGLUnurb *lnurb;
  GLUnurbs* nurb = gluNewNurbsRenderer();
  if (!nurb) 
    luaL_error(L, "glu.NewNurbsRenderer failed");

  lnurb= pushLuaGLUnurb(L);
  lnurb->nurb=nurb;
  lnurb->L=L;
#ifdef GLU_VERSION_1_3
  /* set callback data as userdata object */
  gluNurbsCallbackData(lnurb->nurb,(GLvoid*)lnurb);

  /* create callback table */
  lua_newtable(L);
  lnurb->ref_cb=luaL_ref(L,LUA_REGISTRYINDEX);
#endif
  return 1;
}

static int luaglu_nurbs_gc(lua_State *L) 
{
  LuaGLUnurb * lnurb=checkLuaGLUnurb(L,1);
#ifdef GLU_VERSION_1_3
  luaL_unref(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
#endif
  if (lnurb->nurb) gluDeleteNurbsRenderer(lnurb->nurb);
  return 0;
}

static int luaglu_nurbs_tostring(lua_State *L) 
{
  LuaGLUnurb * lnurb=checkLuaGLUnurb(L,1);
  lua_pushfstring(L,"GLUnurb <%p>",lnurb->nurb);
  return 1;
}


#ifdef GLU_VERSION_1_3

/* NURBS callbacks */

static void nurbBeginDataCB( GLenum type, void *userData ) 
{
  LuaGLUnurb * lnurb=(LuaGLUnurb *) userData;
  lua_State * L=lnurb->L;
  /* push cb table on stack and check for handler*/
  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
  lua_getfield(L,-1,"BeginCB");
  if (lua_isnil(L,-1)) { /* no handler */
    lua_pop(L,2);
    return;
  }
  /* assume udata at stack index 1 */
  lua_pushvalue(L,1); /* push nurb user data */
  lua_pushnumber(L,(lua_Number)type); /* push type */
  lua_call(L,2,0); /* call the handler */
  lua_pop(L,1); /*remove cb table*/
  return;
}

static void nurbVertexDataCB( GLfloat *vertex, void *userData ) 
{
  LuaGLUnurb * lnurb=(LuaGLUnurb *) userData;
  lua_State * L=lnurb->L;
  /* push cb table on stack and check for handler*/
  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
  lua_getfield(L,-1,"VertexCB");
  if (lua_isnil(L,-1)) { /* no handler */
    lua_pop(L,2);
    return;
  }
  /* assume udata at satck index 1 */
  lua_pushvalue(L,1); /* push nurb user data */
  luagl_push_arrayf(L, vertex, 4); 
  lua_call(L,2,0); /* call the handler */
  lua_pop(L,1); /*remove cb table*/
  return;
}

static void nurbNormalDataCB( GLfloat *normal, void *userData ) 
{
  LuaGLUnurb * lnurb=(LuaGLUnurb *) userData;
  lua_State * L=lnurb->L;
  /* push cb table on stack and check for handler*/
  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
  lua_getfield(L,-1,"NormalCB");
  if (lua_isnil(L,-1)) { /* no handler */
    lua_pop(L,2);
    return;
  }
  /* assume udata at satck index 1 */
  lua_pushvalue(L,1); /* push nurb user data */
  luagl_push_arrayf(L, normal, 4); 
  lua_call(L,2,0); /* call the handler */
  lua_pop(L,1); /*remove cb table*/
  return;
}

static void nurbColorDataCB( GLfloat *color, void *userData )
{
  LuaGLUnurb * lnurb=(LuaGLUnurb *) userData;
  lua_State * L=lnurb->L;
  /* push cb table on stack and check for handler*/
  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
  lua_getfield(L,-1,"ColorCB");
  if (lua_isnil(L,-1)) { /* no handler */
    lua_pop(L,2);
    return;
  }
  /* assume udata at satck index 1 */
  lua_pushvalue(L,1); /* push nurb user data */
  luagl_push_arrayf(L, color, 4); 
  lua_call(L,2,0); /* call the handler */
  lua_pop(L,1); /*remove cb table*/
  return;
};

static void nurbTexCoordDataCB( GLfloat *tex_coord, void *userData )
{
  LuaGLUnurb * lnurb=(LuaGLUnurb *) userData;
  lua_State * L=lnurb->L;
  /* push cb table on stack and check for handler*/
  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
  lua_getfield(L,-1,"TexCoordCB");
  if (lua_isnil(L,-1)) { /* no handler */
    lua_pop(L,2);
    return;
  }
  /* assume udata at satck index 1 */
  lua_pushvalue(L,1); /* push nurb user data */
  luagl_push_arrayf(L, tex_coord, 4); 
  lua_call(L,2,0); /* call the handler */
  lua_pop(L,1); /*remove cb table*/
  return;
}
 
static void nurbEndDataCB( void *userData ) 
{
  LuaGLUnurb * lnurb=(LuaGLUnurb *) userData;
  lua_State * L=lnurb->L;
  /* push cb table on stack and check for handler*/
  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);
  lua_getfield(L,-1,"EndCB");
  if (lua_isnil(L,-1)) { /* no handler */
    lua_pop(L,2);
    return;
  }
  /* assume udata at satck index 1 */
  lua_pushvalue(L,1); /* push nurb user data */
  lua_call(L,1,0); /* call the handler */
  lua_pop(L,1); /*remove cb table*/
  return;
};

/* nurb:NurbsCallback(which, func) -> nurb */
static int luaglu_nurbs_callback(lua_State *L) 
{
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  GLenum e;

  e = luaglu_get_gl_enum(L, 2);

  if (!(lua_isfunction(L,3) || lua_isnil(L,3)))
    luaL_argerror(L, 3, "invalid callback");

  lua_rawgeti(L,LUA_REGISTRYINDEX,lnurb->ref_cb);/* get callback table */

  /* set C callback */
  switch(e) 
  {
  case GLU_NURBS_BEGIN : 
    gluNurbsCallback(lnurb->nurb,GLU_NURBS_BEGIN_DATA,nurbBeginDataCB);
    lua_pushstring(L,"BeginCB");lua_pushvalue(L,3);lua_settable(L,-3);
    break;
  case GLU_NURBS_VERTEX : 
    gluNurbsCallback(lnurb->nurb,GLU_NURBS_VERTEX_DATA,nurbVertexDataCB);
    lua_pushstring(L,"VertexCB");lua_pushvalue(L,3);lua_settable(L,-3);
    break;
  case GLU_NURBS_NORMAL : 
    gluNurbsCallback(lnurb->nurb,GLU_NURBS_NORMAL_DATA,nurbNormalDataCB);
    lua_pushstring(L,"NormalCB");lua_pushvalue(L,3);lua_settable(L,-3);
    break;
  case GLU_NURBS_COLOR : 
    gluNurbsCallback(lnurb->nurb,GLU_NURBS_COLOR_DATA,nurbColorDataCB);
    lua_pushstring(L,"ColorCB");lua_pushvalue(L,3);lua_settable(L,-3);
    break;
  case GLU_NURBS_TEXTURE_COORD : 
    gluNurbsCallback(lnurb->nurb,GLU_NURBS_TEXTURE_COORD_DATA,nurbTexCoordDataCB);
    lua_pushstring(L,"TexCoordCB");lua_pushvalue(L,3);lua_settable(L,-3);
    break;
  case GLU_NURBS_END : 
    gluNurbsCallback(lnurb->nurb,GLU_NURBS_END_DATA,nurbEndDataCB);
    lua_pushstring(L,"EndCB");lua_pushvalue(L,3);lua_settable(L,-3);
    break;
  default : 
    luaL_argerror(L, 2, "invalid or unnown enumeration");
  }
  
  lua_pushvalue(L,1);
  return 1;
}
#endif

/* nurb:BeginCurve() -> nurb */
static int luaglu_begin_curve(lua_State *L) 
{ 
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  gluBeginCurve(lnurb->nurb);
  lua_pushvalue(L,1);
  return 1;
}

/* nurb:BeginSurface() -> nurb */
static int luaglu_begin_surface(lua_State *L) 
{ 
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  gluBeginSurface(lnurb->nurb);
  lua_pushvalue(L,1);
  return 1;
}

/* nurb:BeginTrim() -> nurb */
static int luaglu_begin_trim(lua_State *L) 
{ 
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  gluBeginTrim(lnurb->nurb);
  lua_pushvalue(L,1);
  return 1;
}

/* nurb:EndCurve() -> nurb */
static int luaglu_end_curve(lua_State *L) 
{ 
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  gluEndCurve(lnurb->nurb);
  lua_pushvalue(L,1);
  return 1;
}

/* nurb:EndSurface() -> nurb */
static int luaglu_end_surface(lua_State *L) 
{ 
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  gluEndSurface(lnurb->nurb);
  lua_pushvalue(L,1);
  return 1;
}

/* nurb:EndTrim() -> nurb */
static int luaglu_end_trim(lua_State *L) 
{ 
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  gluEndTrim(lnurb->nurb);
  lua_pushvalue(L,1);
  return 1;
}

/* nurb:Property (property) -> value */
/* nurb:Property (property, value) -> nurb */
static int luaglu_get_set_nurbs_property(lua_State *L)
{
  GLenum e;
  GLfloat value;
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);

  e = luaglu_get_gl_enum(L, 2);

  if (lua_isnumber(L, 3))  /* set value */
  { 
    value=(GLfloat)luaL_checknumber(L,3);
    gluNurbsProperty (lnurb->nurb,e,value);
    lua_pushvalue(L,1);
    return 1;
  }
  else
  {
    gluGetNurbsProperty (lnurb->nurb,e,&value);
    lua_pushnumber(L,(lua_Number)value);
    return 1;
  }
}

/* nurb:LoadSamplingMatrices (modelArray, perspectiveArray, viewArray) -> nurb */
static int luaglu_load_sampling_matrices(lua_State *L)
{
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  GLfloat modelMatrix[16];
  GLfloat projMatrix[16]; 
  GLint viewport[4];
  int i;

  if (!lua_istable(L, 2))
    luaL_typerror(L, 2, lua_typename(L, LUA_TTABLE));
  if (!lua_istable(L, 3))
    luaL_typerror(L, 3, lua_typename(L, LUA_TTABLE));
  if (!lua_istable(L, 4))
    luaL_typerror(L, 4, lua_typename(L, LUA_TTABLE));

  if(luaL_getn(L, 2) < 16)
    luaL_argerror(L, 2, "invalid number of items in table (n<16)");
  if(luaL_getn(L, 3) < 16)
    luaL_argerror(L, 3, "invalid number of items in table (n<16)");
  if(luaL_getn(L, 4) < 4)
    luaL_argerror(L, 4, "invalid number of items in table (n<4)");

  for (i=0;i<16;i++) 
  {
    lua_pushnumber(L,i);
    lua_gettable(L,2);
    modelMatrix[i]=(GLfloat)luaL_checknumber(L,-1);
    lua_pop(L,1);
  }

  for (i=0;i<16;i++) 
  {
    lua_pushnumber(L,i);
    lua_gettable(L,3);
    projMatrix[i]=(GLfloat)luaL_checknumber(L,-1);
    lua_pop(L,1);
  }

  for (i=0;i<4;i++) 
  {
    lua_pushnumber(L,i);
    lua_gettable(L,4);
    viewport[i]=luaL_checkinteger(L,-1);
    lua_pop(L,1);
  }

  gluLoadSamplingMatrices(lnurb->nurb,modelMatrix,projMatrix,viewport);

  lua_pushvalue(L,1);
  return 1;
}

/* nurb:Curve (knotsArray, controlArray, type) -> nurb */
static int luaglu_nurbs_curve(lua_State *L)
{
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  GLint ptsCount=0;
  GLint knotCount=0;
  GLfloat *knots;
  GLint stride;
  GLfloat *points;
  GLint order;
  int size=1;
  GLenum e;

  e = luaglu_get_gl_enum(L, 4);

  switch(e)
  {
  case GL_MAP1_INDEX:
  case GL_MAP2_INDEX:
  case GL_MAP1_TEXTURE_COORD_1:
  case GL_MAP2_TEXTURE_COORD_1:
    size = 1;
    break;
  case GLU_MAP1_TRIM_2:
  case GL_MAP1_TEXTURE_COORD_2:
  case GL_MAP2_TEXTURE_COORD_2:
    size = 2;
    break;
  case GLU_MAP1_TRIM_3:
  case GL_MAP1_VERTEX_3:
  case GL_MAP2_VERTEX_3:
  case GL_MAP1_NORMAL:
  case GL_MAP2_NORMAL:
  case GL_MAP1_TEXTURE_COORD_3:
  case GL_MAP2_TEXTURE_COORD_3:
    size = 3;
    break;
  case GL_MAP1_VERTEX_4:
  case GL_MAP2_VERTEX_4:
  case GL_MAP1_COLOR_4:
  case GL_MAP2_COLOR_4:
  case GL_MAP1_TEXTURE_COORD_4:
  case GL_MAP2_TEXTURE_COORD_4:
    size = 4;
    break;
  }

  knotCount=luagl_get_arrayf(L, 2, &knots);
  ptsCount=luagl_get_arrayf(L, 3, &points);

  ptsCount/=size;
  stride=size;

  order=knotCount-ptsCount;

  if(order<1)
  {
    LUAGL_DELETE_ARRAY(knots);
    LUAGL_DELETE_ARRAY(points);
    luaL_error(L,"incorrect order (<1)");
  }

  gluNurbsCurve (lnurb->nurb,knotCount,knots,stride,points,order,e);

  LUAGL_DELETE_ARRAY(knots);
  LUAGL_DELETE_ARRAY(points);

  lua_pushvalue(L,1);
  return 1;
}

/* nurb:Surface (sKnotsArray, tKnotsArray, controlArray, type) -> nurb */
static int luaglu_nurbs_surface(lua_State *L) 
{
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  GLint sKnotCount=0;
  GLint tKnotCount=0;
  GLfloat *sKnots;
  GLfloat *tKnots;
  GLint sStride,tStride;
  GLint sPtsCount,tPtsCount;
  GLfloat *points;
  GLint sOrder,tOrder;
  GLenum e;
  int size=1;

  e = luaglu_get_gl_enum(L, 5);

  switch(e)
  {
  case GL_MAP1_INDEX:
  case GL_MAP2_INDEX:
  case GL_MAP1_TEXTURE_COORD_1:
  case GL_MAP2_TEXTURE_COORD_1:
    size = 1;
    break;
  case GLU_MAP1_TRIM_2:
  case GL_MAP1_TEXTURE_COORD_2:
  case GL_MAP2_TEXTURE_COORD_2:
    size = 2;
    break;
  case GLU_MAP1_TRIM_3:
  case GL_MAP1_VERTEX_3:
  case GL_MAP2_VERTEX_3:
  case GL_MAP1_NORMAL:
  case GL_MAP2_NORMAL:
  case GL_MAP1_TEXTURE_COORD_3:
  case GL_MAP2_TEXTURE_COORD_3:
    size = 3;
    break;
  case GL_MAP1_VERTEX_4:
  case GL_MAP2_VERTEX_4:
  case GL_MAP1_COLOR_4:
  case GL_MAP2_COLOR_4:
  case GL_MAP1_TEXTURE_COORD_4:
  case GL_MAP2_TEXTURE_COORD_4:
    size = 4;
    break;
  }

  sKnotCount=luagl_get_arrayf(L, 2, &sKnots);
  tKnotCount=luagl_get_arrayf(L, 3, &tKnots);
  tPtsCount = luagl_get_array2f(L, 4, &points, &sPtsCount);
  sPtsCount /= size;

  sStride=size;
  tStride=size*sPtsCount;

  sOrder=sKnotCount-sPtsCount;
  tOrder=tKnotCount-tPtsCount;
  
  if (sOrder<1 || tOrder<1)
  {
    LUAGL_DELETE_ARRAY(sKnots);
    LUAGL_DELETE_ARRAY(tKnots);
    LUAGL_DELETE_ARRAY(points);
    luaL_error(L,"incorrect order");
  }

  gluNurbsSurface (lnurb->nurb,sKnotCount,sKnots,tKnotCount,tKnots,sStride,tStride,points,sOrder,tOrder,e); 

  LUAGL_DELETE_ARRAY(sKnots);
  LUAGL_DELETE_ARRAY(tKnots);
  LUAGL_DELETE_ARRAY(points);

  lua_pushvalue(L,1);
  return 1;
}

/* nurb:PwlCurve (dataArray, type) -> nurb */
static int luaglu_pwl_curve(lua_State *L)
{
  LuaGLUnurb *lnurb=checkLuaGLUnurb(L,1);
  GLint ptsCount=0;
  GLint stride;
  GLfloat *points;
  int size=1;
  GLenum e;

  e = luaglu_get_gl_enum(L, 3);

  switch(e)
  {
  case GL_MAP1_INDEX:
  case GL_MAP2_INDEX:
  case GL_MAP1_TEXTURE_COORD_1:
  case GL_MAP2_TEXTURE_COORD_1:
    size = 1;
    break;
  case GLU_MAP1_TRIM_2:
  case GL_MAP1_TEXTURE_COORD_2:
  case GL_MAP2_TEXTURE_COORD_2:
    size = 2;
    break;
  case GLU_MAP1_TRIM_3:
  case GL_MAP1_VERTEX_3:
  case GL_MAP2_VERTEX_3:
  case GL_MAP1_NORMAL:
  case GL_MAP2_NORMAL:
  case GL_MAP1_TEXTURE_COORD_3:
  case GL_MAP2_TEXTURE_COORD_3:
    size = 3;
    break;
  case GL_MAP1_VERTEX_4:
  case GL_MAP2_VERTEX_4:
  case GL_MAP1_COLOR_4:
  case GL_MAP2_COLOR_4:
  case GL_MAP1_TEXTURE_COORD_4:
  case GL_MAP2_TEXTURE_COORD_4:
    size = 4;
    break;
  }

  ptsCount=luagl_get_arrayf(L, 2, &points);
  ptsCount/=size;
  stride=size;

  gluPwlCurve (lnurb->nurb,ptsCount,points,stride,e);

  LUAGL_DELETE_ARRAY(points);

  lua_pushvalue(L,1);
  return 1;
}


/* Nurbs related functions are implemented as Nurbs userdata methods */
static const luaL_reg luaglunurb_methods[] = {
  {"__gc",luaglu_nurbs_gc},
  {"__tostring",luaglu_nurbs_tostring},

#ifdef GLU_VERSION_1_3
  {"Callback",luaglu_nurbs_callback},
#endif
  {"Property",luaglu_get_set_nurbs_property},
  {"LoadSamplingMatrices",luaglu_load_sampling_matrices},
  {"BeginCurve",luaglu_begin_curve},
  {"BeginSurface",luaglu_begin_surface},
  {"BeginTrim",luaglu_begin_trim},
  {"EndCurve",luaglu_end_curve},
  {"EndSurface",luaglu_end_surface},
  {"EndTrim",luaglu_end_trim},
  {"Curve",luaglu_nurbs_curve},
  {"Surface",luaglu_nurbs_surface},
  {"PwlCurve",luaglu_pwl_curve},
  {NULL, NULL}
};


static const luaL_reg luaglu_lib[] = {
  {"GetString", luaglu_get_string},
  {"Ortho2D", luaglu_ortho_2D},
  {"Perspective", luaglu_perspective},
  {"LookAt", luaglu_look_at},
  {"PickMatrix", luaglu_pick_matrix},
  {"Build2DMipmaps", luaglu_build_2d_mipmaps},
  {"ErrorString", luaglu_error_string},

  /* Quadric */
  {"NewQuadric", luaglu_new_quadric},

  /* Nurb */
  {"NewNurbsRenderer", luaglu_new_nurbs},

  {NULL, NULL}
};

int luaopen_luaglu(lua_State *L)
{
  /* metatable for GLUquadric udata*/ 
  luaL_newmetatable(L,LUAGLUQUADRIC);
  lua_pushstring(L,"__index");
  lua_pushvalue(L,-2);
  lua_settable(L,-3);
  luaL_register(L,0,luagluquadric_methods);

  /* metatable for GLUnurb udata*/ 
  luaL_newmetatable(L,LUAGLUNURB);
  lua_pushstring(L,"__index");
  lua_pushvalue(L,-2);
  lua_settable(L,-3);
  luaL_register(L,0,luaglunurb_methods);

  luaL_register(L, "glu", luaglu_lib);
  luagl_initconst(L, luaglu_const);

  lua_pushstring(L, "TESS_MAX_COORD");
  lua_pushnumber(L, GLU_TESS_MAX_COORD);
  lua_settable(L, -3);

  return 1;
}
