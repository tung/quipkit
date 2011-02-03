#include "options.h"

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
