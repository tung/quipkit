#include "options.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"



#define ERROR_MSG_LENGTH 500

static char m_error_message[ERROR_MSG_LENGTH];

#define OPT_SET_ERROR(...) snprintf(m_error_message, ERROR_MSG_LENGTH, __VA_ARGS__)



static void opt_InitOptions(opt_Options *o) {
    o->has_script = 0;
    o->has_config = 0;
    o->has_base = 0;
    o->has_width = 0;
    o->has_height = 0;
    o->has_fullscreen = 0;
    o->has_channels = 0;
}


const char *opt_GetError() {
    return m_error_message;
}


int opt_ParseOptions(int argc, char *argv[], opt_Options *opts, int *script_args_start) {
    #define PO_DUPCHECK(po_opt) \
        do { \
            if (opts->has_##po_opt) { \
                OPT_SET_ERROR("--" #po_opt " can't be given twice"); \
                return 1; \
            } \
        } while (0)
    #define PO_NEXTARG(po_opt, po_err_arg_needed) \
        do { \
            ++o; \
            if (o >= argc) { \
                OPT_SET_ERROR("--" #po_opt " needs " po_err_arg_needed); \
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
                OPT_SET_ERROR("--" #po_opt " argument must be 0 (off) or 1 (on)"); \
                return 1; \
            } \
            opts->has_##po_opt = 1; \
        } while(0)
    #define PO_GETNUMBER(po_opt, po_cmp, po_num) \
        do { \
            opts->po_opt = atoi(argv[o]); \
            if (!(opts->po_opt po_cmp po_num)) { \
                OPT_SET_ERROR("--" #po_opt " must be " #po_cmp " " #po_num); \
                return 1; \
            } \
            opts->has_##po_opt = 1; \
        } while(0)
    #define PO_GETPATH(po_opt) \
        do { \
            if (fs_AbsolutePath(argv[o], opts->po_opt, MAX_PATH)) { \
                OPT_SET_ERROR("no such --" #po_opt " path: %s", argv[o]); \
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
            OPT_SET_ERROR("Usage: ./game <args> -- <script args>\n"
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
                    "Script args are passed to the game script.");
            return -1;
        } else if (strcmp(argv[o], "--version") == 0) {
            /* TODO: Print the actual version. */
            OPT_SET_ERROR("Quipkit 0.0.0");
            return -1;
        } else if (strcmp(argv[o], "--") == 0) {
            *script_args_start = o + 1;
            break;
        } else if (strncmp(argv[o], "--", 2) == 0) {
            OPT_SET_ERROR("Unknown option %s given", argv[o]);
            return 1;
        } else {
            if (o > 1) {
                OPT_SET_ERROR("Argument %s not associated with any option", argv[o]);
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


int opt_LoadConfig(const opt_Options *cmd_line_opts, opt_Options *config_opts) {
    #define LC_GET_FLAG(lc_opt) \
        do { \
            lua_getfield(L, -1, #lc_opt); \
            if (lua_isnumber(L, -1)) { \
                int lc_opt = lua_tointeger(L, -1); \
                if (lc_opt == 1) { \
                    config_opts->lc_opt = 1; \
                } else if (lc_opt == 0) { \
                    config_opts->lc_opt = 0; \
                } else { \
                    lua_close(L); \
                    OPT_SET_ERROR("'" #lc_opt "' number must be 0 (off) or 1 (on)"); \
                    return 1; \
                } \
                config_opts->has_##lc_opt = 1; \
            } else if (lua_isboolean(L, -1)) { \
                if (lua_toboolean(L, -1)) { \
                    config_opts->lc_opt = 1; \
                } else { \
                    config_opts->lc_opt = 0; \
                } \
                config_opts->has_##lc_opt = 1; \
            } else if (!lua_isnil(L, -1)) { \
                lua_close(L); \
                OPT_SET_ERROR("'" #lc_opt "' neither a number or a boolean"); \
                return 1; \
            } \
            lua_pop(L, 1); \
        } while (0)
    #define LC_GET_NUMBER(lc_opt, lc_cond, lc_value) \
        do { \
            lua_getfield(L, -1, #lc_opt); \
            if (lua_isnumber(L, -1)) { \
                if (!((config_opts->lc_opt = lua_tointeger(L, -1)) lc_cond lc_value)) { \
                    lua_close(L); \
                    OPT_SET_ERROR("'" #lc_opt "' must be " #lc_cond " " #lc_value); \
                    return 1; \
                } \
                config_opts->has_##lc_opt = 1; \
            } else if (!lua_isnil(L, -1)) { \
                lua_close(L); \
                OPT_SET_ERROR("'" #lc_opt "' not a number"); \
                return 1; \
            } \
            lua_pop(L, 1); \
        } while (0)
    #define LC_GET_PATH(lc_opt) \
        do { \
            lua_getfield(L, -1, #lc_opt); \
            if (lua_isstring(L, -1)) { \
                if (fs_AbsolutePath(lua_tostring(L, -1), config_opts->lc_opt, MAX_PATH)) { \
                    lua_close(L); \
                    OPT_SET_ERROR("'" #lc_opt "' path not found"); \
                    return 1; \
                } \
                config_opts->has_##lc_opt = 1; \
            } else if (!(lua_isnil(L, -1))) { \
                lua_close(L); \
                OPT_SET_ERROR("'" #lc_opt "' not a string"); \
                return 1; \
            } \
            lua_pop(L, 1); \
        } while (0)

    opt_InitOptions(config_opts);

    lua_State *L = luaL_newstate();
    if (L == NULL) {
        OPT_SET_ERROR("couldn't prepare Lua state to read config file");
        return 1;
    }

    luaL_openlibs(L);

    char conf_lua[MAX_PATH];
    fs_EnginePath(conf_lua, MAX_PATH);
    fs_Append(conf_lua, MAX_PATH, "conf.lua");
    if (luaL_loadfile(L, conf_lua)) {
        OPT_SET_ERROR("couldn't load conf.lua config bootstrap");
        lua_close(L);
        return 1;
    }

    lua_createtable(L, 0, 2);
    if (cmd_line_opts->has_config) {
        lua_pushstring(L, cmd_line_opts->config);
        lua_setfield(L, -2, "config");
    }
    if (cmd_line_opts->has_script) {
        lua_pushstring(L, cmd_line_opts->script);
    } else {
        char default_script[MAX_PATH];
        fs_EnginePath(default_script, MAX_PATH);
        fs_Append(default_script, MAX_PATH, "startup/main.lua");
        lua_pushstring(L, default_script);
    }
    lua_setfield(L, -2, "script");
    if (lua_pcall(L, 1, 1, 0)) {
        OPT_SET_ERROR("%s", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    /* Get the game config from the returned table. */
    if (lua_isnil(L, -1)) {
        lua_close(L);
        return 0;
    }
    LC_GET_PATH(base);
    LC_GET_NUMBER(width, >, 0);
    LC_GET_NUMBER(height, >, 0);
    LC_GET_FLAG(fullscreen);
    LC_GET_NUMBER(channels, >=, 0);

    lua_close(L);
    return 0;

    #undef LC_GET_FLAG
    #undef LC_GET_NUMBER
    #undef LC_GET_PATH
}
