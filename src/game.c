#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

#include "fs.h"
#include "options.h"


static void DumpOptions(const char *label, const opt_Options *opts) {
    printf("%s:\n", label);
    if (opts->has_script) { printf("script: %s\n", opts->script); }
    if (opts->has_config) { printf("config: %s\n", opts->config); }
    if (opts->has_base) { printf("base: %s\n", opts->base); }
    if (opts->has_width) { printf("width: %d\n", opts->width); }
    if (opts->has_height) { printf("height: %d\n", opts->height); }
    if (opts->has_fullscreen) { printf("fullscreen: %d\n", opts->fullscreen); }
    if (opts->has_channels) { printf("channels: %d\n", opts->channels); }
}


static int GetConfig(int argc, char *argv[], opt_Options *cmd_line_opts, opt_Options *config_opts, int *script_args_start) {
    int po_ret = opt_ParseOptions(argc, argv, cmd_line_opts, script_args_start);
    if (po_ret) {
        printf("%s\n", opt_GetError());
        if (po_ret == -1) { return -1; }
        return 1;
    }
    DumpOptions("Command line options", cmd_line_opts);

    int lc_ret = opt_LoadConfig(cmd_line_opts, config_opts);
    if (lc_ret) {
        printf("%s\n", opt_GetError());
        return 1;
    }
    DumpOptions("Config options", config_opts);

    if (*script_args_start < argc) {
        printf("Script args:");
        for (int i = *script_args_start; i < argc; ++i) {
            printf(" %s", argv[i]);
        }
        printf("\n");
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
    if (luaL_loadfile(L, opts->script)) {
        fprintf(stderr, "couldn't load game script: %s\n", lua_tostring(L, -1));
        lua_pop(L, 2);
        return 1;
    }

    for (int i = script_args_start; i < argc; ++i) {
        lua_pushstring(L, argv[i]);
    }

    int num_args = argc - script_args_start;
    if (lua_pcall(L, num_args, 0, num_args - 2)) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_gc(L, LUA_GCCOLLECT, 0);
        return 1;
    }
    lua_pop(L, 1);

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
