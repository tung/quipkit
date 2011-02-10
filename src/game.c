/**
 * See copyright notice in doc/COPYRIGHT.
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

#include "fs.h"
#include "options.h"


static int GetConfig(int argc, char *argv[], opt_Options *cmd_line_opts, opt_Options *config_opts, int *script_args_start) {
    int po_ret = opt_ParseOptions(argc, argv, cmd_line_opts, script_args_start);
    if (po_ret) {
        printf("%s\n", opt_GetError());
        if (po_ret == -1) { return -1; }
        return 1;
    }

    int lc_ret = opt_LoadConfig(cmd_line_opts, config_opts);
    if (lc_ret) {
        printf("%s\n", opt_GetError());
        return 1;
    }

    return 0;
}


static int TracebackLua(lua_State *L) {
    if (!lua_isstring(L, 1)) { return 1; }
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2);
        return 1;
    }
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);
    return 1;
}


static int LoadApi(lua_State *L) {
    char engine_path[MAX_PATH];
    if (fs_EnginePath(engine_path, MAX_PATH)) {
        fprintf(stderr, "couldn't get engine path to load Quipkit's API\n");
        return 1;
    }
    if (fs_ChDir(engine_path)) {
        fprintf(stderr, "couldn't change directory to engine path to load Quipkit's API\n");
        return 1;
    }

    lua_pushcfunction(L, TracebackLua);
    if (luaL_loadfile(L, "api.lua")) {
        fprintf(stderr, "%s\ncouldn't load api.lua bootstrap script\n", lua_tostring(L, -1));
        return 1;
    }
    if (lua_pcall(L, 0, 0, -2)) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_gc(L, LUA_GCCOLLECT, 0);
        return 1;
    }
    lua_pop(L, 1);

    return 0;
}


static int RunGame(lua_State *L, opt_Options *opts, int argc, char *argv[], int script_args_start) {
    if (fs_ChDir(opts->base)) {
        fprintf(stderr, "couldn't change to base directory %s\n", opts->base);
        return 1;
    }

    lua_pushcfunction(L, TracebackLua);
    int traceback_fn = lua_gettop(L);

    /* Load init and close functions to be called around game script chunk. */
    char wrap_path[MAX_PATH];
    if (fs_EnginePath(wrap_path, MAX_PATH) || fs_Append(wrap_path, MAX_PATH, "wrap.lua")) {
        fprintf(stderr, "couldn't build path to wrap.lua wrapper functions\n");
        lua_pop(L, 1);
        return 1;
    }
    if (luaL_loadfile(L, wrap_path)) {
        fprintf(stderr, "couldn't load wrap.lua wrapper functions: %s\n", lua_tostring(L, -1));
        lua_pop(L, 2);
        return 1;
    }
    lua_createtable(L, 0, 4);
    lua_pushinteger(L, opts->width);
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, opts->height);
    lua_setfield(L, -2, "height");
    lua_pushboolean(L, opts->fullscreen);
    lua_setfield(L, -2, "fullscreen");
    lua_pushinteger(L, opts->channels);
    lua_setfield(L, -2, "channels");
    if (lua_pcall(L, 1, 2, traceback_fn)) {
        fprintf(stderr, "wrapper load error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 2);
        return 1;
    }
    int init_fn = lua_gettop(L) - 1;
    int close_fn = lua_gettop(L);
    if (!lua_isfunction(L, init_fn) || !lua_isfunction(L, close_fn)) {
        fprintf(stderr, "wrap.lua didn't return init and close functions\n");
        lua_pop(L, 3);
        return 1;
    }

    /* Call init function from wrap.lua. */
    lua_pushvalue(L, init_fn);
    if (lua_pcall(L, 0, 0, traceback_fn)) {
        fprintf(stderr, "init error: %s\n", lua_tostring(L, -1));
        /* Pop: traceback, init, close, error */
        lua_pop(L, 4);
        return 1;
    }

    /* Load and run the game script. */
    if (luaL_loadfile(L, opts->script)) {
        fprintf(stderr, "couldn't load game script: %s\n", lua_tostring(L, -1));
        lua_pushvalue(L, close_fn);
        if (lua_pcall(L, 0, 0, traceback_fn)) {
            fprintf(stderr, "close error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        /* Pop: traceback, init, close, error */
        lua_pop(L, 4);
        return 1;
    }
    for (int i = script_args_start; i < argc; ++i) {
        lua_pushstring(L, argv[i]);
    }
    int num_args = argc - script_args_start;
    if (lua_pcall(L, num_args, 0, traceback_fn)) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pushvalue(L, close_fn);
        if (lua_pcall(L, 0, 0, traceback_fn)) {
            fprintf(stderr, "close error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        /* Pop: traceback, init, close, error */
        lua_pop(L, 4);
        return 1;
    }

    /* Call close function from wrap.lua. */
    lua_pushvalue(L, close_fn);
    if (lua_pcall(L, 0, 0, traceback_fn)) {
        fprintf(stderr, "close error: %s\n", lua_tostring(L, -1));
        /* Pop: traceback, init, close, error */
        lua_pop(L, 4);
        return 1;
    }

    /* Pop: traceback, init, close */
    lua_pop(L, 3);

    return 0;
}


int main(int argc, char *argv[]) {
    opt_Options cmd_line_opts, config_opts, final_opts;
    int script_args_start;

    int gc_ret = GetConfig(argc, argv, &cmd_line_opts, &config_opts, &script_args_start);
    switch (gc_ret) {
    case -1: return 0;
    case 1: return 1;
    default: break;
    }

    lua_State *L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "couldn't prepare Lua state for game\n");
        return 1;
    }

    luaL_openlibs(L);

    if (LoadApi(L)) {
        lua_close(L);
        return 1;
    }

    if (opt_ConfigGame(&cmd_line_opts, &config_opts, &final_opts)) {
        lua_close(L);
        return 1;
    }

    if (RunGame(L, &final_opts, argc, argv, script_args_start)) {
        lua_close(L);
        return 1;
    }

    lua_close(L);
    return 0;
}
