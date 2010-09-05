#ifndef SDLGL_H_INCLUDED
#define SDLGL_H_INCLUDED

/** Everything below was swiped from Code::Blocks' DLL template. */

#include <lua.h>
#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

int DLL_EXPORT luaopen_sdlgl(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif
