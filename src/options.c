#include "options.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"


static void opt_InitOptions(opt_Options *o) {
    o->has_script = 0;
    o->has_config = 0;
    o->has_base = 0;
    o->has_width = 0;
    o->has_height = 0;
    o->has_fullscreen = 0;
    o->has_channels = 0;
}


int opt_ParseOptions(int argc, char *argv[], opt_Options *opts, int *script_args_start) {
    #define PO_DUPCHECK(po_opt) \
        do { \
            if (opts->has_##po_opt) { \
                fprintf(stderr, "--" #po_opt " can't be given twice\n"); \
                return 1; \
            } \
        } while (0)
    #define PO_NEXTARG(po_opt, po_err_arg_needed) \
        do { \
            ++o; \
            if (o >= argc) { \
                fprintf(stderr, "--" #po_opt " needs " po_err_arg_needed "\n"); \
                return 1; \
            } \
        } while(0)

    #define PO_GETFLAG(po_opt) \
        do { \
            if (strcmp(argv[o], "0") == 0) { \
                opts->po_opt = 0; \
            } else if (strcmp(argv[o], "1") == 0) { \
                opts->po_opt = 1; \
            } else { \
                fprintf(stderr, "--" #po_opt " argument must be 0 (off) or 1 (on)\n"); \
                return 1; \
            } \
            opts->has_##po_opt = 1; \
        } while(0)
    #define PO_GETNUMBER(po_opt, po_cmp, po_num) \
        do { \
            opts->po_opt = atoi(argv[o]); \
            if (!(opts->po_opt po_cmp po_num)) { \
                fprintf(stderr, "--" #po_opt " must be " #po_cmp " " #po_num "\n"); \
                return 1; \
            } \
            opts->has_##po_opt = 1; \
        } while(0)
    #define PO_GETPATH(po_opt) \
        do { \
            if (fs_AbsolutePath(argv[o], opts->po_opt, MAX_PATH)) { \
                fprintf(stderr, "no such --" #po_opt " path: %s\n", argv[o]); \
                return 1; \
            } \
            opts->has_##po_opt = 1; \
        } while (0)

    #define PO_FLAG(po_opt) \
        do { \
            PO_DUPCHECK(po_opt); \
            PO_NEXTARG(po_opt, "an argument"); \
            PO_GETFLAG(po_opt); \
        } while (0)
    #define PO_NUMBER(po_opt, po_cmp, po_num) \
        do { \
            PO_DUPCHECK(po_opt); \
            PO_NEXTARG(po_opt, "a number argument"); \
            PO_GETNUMBER(po_opt, po_cmp, po_num); \
        } while (0)
    #define PO_PATH(po_opt) \
        do { \
            PO_DUPCHECK(po_opt); \
            PO_NEXTARG(po_opt, "a path argument"); \
            PO_GETPATH(po_opt); \
        } while(0)

    opt_InitOptions(opts);
    *script_args_start = argc;
    int o = 1;
    while (o < argc) {
        if (strcmp(argv[o], "--script") == 0) {
            PO_PATH(script);
        } else if (strcmp(argv[o], "--config") == 0) {
            PO_PATH(config);
        } else if (strcmp(argv[o], "--base") == 0) {
            PO_PATH(base);
        } else if (strcmp(argv[o], "--width") == 0) {
            PO_NUMBER(width, >, 0);
        } else if (strcmp(argv[o], "--height") == 0) {
            PO_NUMBER(height, >, 0);
        } else if (strcmp(argv[o], "--fullscreen") == 0) {
            PO_FLAG(fullscreen);
        } else if (strcmp(argv[o], "--channels") == 0) {
            PO_NUMBER(channels, >, -1);
        } else if (strcmp(argv[o], "--help") == 0) {
            printf("Usage: ./game <args> -- <script args>\n"
                    "args:\n"
                    "    --script     <path to game script>\n"
                    "    --config     <path to config file>\n"
                    "    --base       <path from which assets/scripts are loaded>\n"
                    "    --width      <game screen width>\n"
                    "    --height     <game screen height>\n"
                    "    --fullscreen <1 for yes, 0 for no>\n"
                    "    --channels   <number of sound channels>\n"
                    "    --help       print this message and exit\n"
                    "    --version    print the version of the Quipkit engine and exit\n"
                    "Script args are passed to the game script.\n");
            return 2;
        } else if (strcmp(argv[o], "--version") == 0) {
            /* TODO: Print the actual version. */
            printf("Quipkit 0.0.0\n");
            return 2;
        } else if (strcmp(argv[o], "--") == 0) {
            *script_args_start = o + 1;
            break;
        } else if (strncmp(argv[o], "--", 2) == 0) {
            fprintf(stderr, "Unknown option %s given\n", argv[o]);
            return 1;
        } else {
            if (o > 1) {
                fprintf(stderr, "Argument %s not associated with any option\n", argv[o]);
                return 1;
            }
            /* Short-form arguments: ./game script arg1 arg2 arg3 ... */
            fs_AbsolutePath(argv[o], opts->script, MAX_PATH);
            opts->has_script = 1;
            *script_args_start = o + 1;
            break;
        }
        ++o;
    }
    return 0;

    #undef PO_DUPCHECK
    #undef PO_NEXTARG
    #undef PO_GETFLAG
    #undef PO_GETNUMBER
    #undef PO_GETPATH
    #undef PO_FLAG
    #undef PO_NUMBER
    #undef PO_PATH
}


/**
 * Return values:
 * 0    OK
 * 1    couldn't init new Lua state to read config file
 * 2    conf.lua missing
 * 3    given config file missing or error in config file
 * 4    config file 'base' path not found
 * 5    config file 'base' not a string
 * 6    config file 'width' <= 0
 * 7    config file 'width' not a number
 * 8    config file 'height' <= 0
 * 9    config file 'height' not a number
 * 10   config file 'fullscreen' was a number other than 0 (off) or 1 (on)
 * 11   config file 'fullscreen' was not a number or a boolean
 * 12   config file 'channels' < 0
 * 13   config file 'channels' not a number
 */
int opt_LoadConfig(const opt_Options *cmd_line_opts, opt_Options *config_opts) {
    opt_InitOptions(config_opts);

    lua_State *L = luaL_newstate();
    if (L == NULL) { return 1; }

    luaL_openlibs(L);

    if (luaL_loadfile(L, "conf.lua")) {
        lua_close(L);
        return 2;
    }

    lua_createtable(L, 0, 2);
    if (cmd_line_opts->has_config) {
        lua_pushstring(L, cmd_line_opts->config);
        lua_setfield(L, -2, "config");
    }
    if (cmd_line_opts->has_script) {
        lua_pushstring(L, cmd_line_opts->script);
        lua_setfield(L, -2, "script");
    }
    if (lua_pcall(L, 1, 1, 0)) {
        lua_close(L);
        return 3;
    }

    if (lua_isnil(L, -1)) {
        lua_close(L);
        return 0;
    }

    /* Get the game config from the returned table. */
    lua_getfield(L, -1, "base");
    if (lua_isstring(L, -1)) {
        if (fs_AbsolutePath(lua_tostring(L, -1), config_opts->base, MAX_PATH)) {
            lua_close(L);
            return 4;
        }
        config_opts->has_base = 1;
    } else if (!lua_isnil(L, -1)) {
        lua_close(L);
        return 5;
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "width");
    if (lua_isnumber(L, -1)) {
        if ((config_opts->width = lua_tointeger(L, -1)) <= 0) {
            lua_close(L);
            return 6;
        }
        config_opts->has_width = 1;
    } else if (!lua_isnil(L, -1)) {
        lua_close(L);
        return 7;
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "height");
    if (lua_isnumber(L, -1)) {
        if ((config_opts->height = lua_tointeger(L, -1)) <= 0) {
            lua_close(L);
            return 8;
        }
        config_opts->has_height = 1;
    } else if (!lua_isnil(L, -1)) {
        lua_close(L);
        return 9;
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "fullscreen");
    if (lua_isnumber(L, -1)) {
        int fullscreen = lua_tointeger(L, -1);
        if (fullscreen == 1) {
            config_opts->fullscreen = 1;
        } else if (fullscreen == 0) {
            config_opts->fullscreen = 0;
        } else {
            lua_close(L);
            return 10;
        }
        config_opts->has_fullscreen = 1;
    } else if (lua_isboolean(L, -1)) {
        if (lua_toboolean(L, -1)) {
            config_opts->fullscreen = 1;
        } else {
            config_opts->fullscreen = 0;
        }
        config_opts->has_fullscreen = 1;
    } else if (!lua_isnil(L, -1)) {
        lua_close(L);
        return 11;
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "channels");
    if (lua_isnumber(L, -1)) {
        if ((config_opts->channels = lua_tointeger(L, -1)) < 0) {
            lua_close(L);
            return 12;
        }
        config_opts->has_channels = 1;
    } else if (!lua_isnil(L, -1)) {
        lua_close(L);
        return 13;
    }
    lua_pop(L, 1);

    lua_close(L);
    return 0;
}
