#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_PATH 256


typedef struct {
    char has_script;
    char script[MAX_PATH];
    char has_config;
    char config[MAX_PATH];
    char has_base;
    char base[MAX_PATH];
    char has_width;
    int  width;
    char has_height;
    int  height;
    char has_fullscreen;
    char fullscreen;
    char has_channels;
    int  channels;
} Options;


void Options_init(Options *o) {
    o->has_script = 0;
    o->has_config = 0;
    o->has_base = 0;
    o->has_width = 0;
    o->has_height = 0;
    o->has_fullscreen = 0;
    o->has_channels = 0;
}


/* TODO: Move this elsewhere and implement it. */
void fs_AbsolutePath(char *path, char *dest, int dest_size) {
    strncpy(dest, path, dest_size);
}


static int ParseOptions(int argc, char *argv[], Options *opts, int *script_args_start) {
    #define ParseOptions_DUPCHECK(po_opt) \
        do { \
            if (opts->has_##po_opt) { \
                fprintf(stderr, "--" #po_opt " can't be given twice\n"); \
                return 1; \
            } \
        } while (0)
    #define ParseOptions_NEXTARG(po_opt, po_err_arg_needed) \
        do { \
            ++o; \
            if (o >= argc) { \
                fprintf(stderr, "--" #po_opt " needs " po_err_arg_needed "\n"); \
                return 1; \
            } \
        } while(0)
    #define ParseOptions_GETFLAG(po_opt) \
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
    #define ParseOptions_GETNUMBER(po_opt, po_cmp, po_num) \
        do { \
            opts->po_opt = atoi(argv[o]); \
            if (!(opts->po_opt po_cmp po_num)) { \
                fprintf(stderr, "--" #po_opt " must be " #po_cmp " " #po_num "\n"); \
                return 1; \
            } \
            opts->has_##po_opt = 1; \
        } while(0)
    #define ParseOptions_GETPATH(po_opt) \
        do { \
            fs_AbsolutePath(argv[o], opts->po_opt, MAX_PATH); \
            opts->has_##po_opt = 1; \
        } while (0)
    #define ParseOptions_FLAG(po_opt) \
        do { \
            ParseOptions_DUPCHECK(po_opt); \
            ParseOptions_NEXTARG(po_opt, "an argument"); \
            ParseOptions_GETFLAG(po_opt); \
        } while (0)
    #define ParseOptions_NUMBER(po_opt, po_cmp, po_num) \
        do { \
            ParseOptions_DUPCHECK(po_opt); \
            ParseOptions_NEXTARG(po_opt, "a number argument"); \
            ParseOptions_GETNUMBER(po_opt, po_cmp, po_num); \
        } while (0)
    #define ParseOptions_PATH(po_opt) \
        do { \
            ParseOptions_DUPCHECK(po_opt); \
            ParseOptions_NEXTARG(po_opt, "a path argument"); \
            ParseOptions_GETPATH(po_opt); \
        } while(0)

    Options_init(opts);
    *script_args_start = argc;
    int o = 1;
    while (o < argc) {
        if (strcmp(argv[o], "--script") == 0) {
            ParseOptions_PATH(script);
        } else if (strcmp(argv[o], "--config") == 0) {
            ParseOptions_PATH(config);
        } else if (strcmp(argv[o], "--base") == 0) {
            ParseOptions_PATH(base);
        } else if (strcmp(argv[o], "--width") == 0) {
            ParseOptions_NUMBER(width, >, 0);
        } else if (strcmp(argv[o], "--height") == 0) {
            ParseOptions_NUMBER(height, >, 0);
        } else if (strcmp(argv[o], "--fullscreen") == 0) {
            ParseOptions_FLAG(fullscreen);
        } else if (strcmp(argv[o], "--channels") == 0) {
            ParseOptions_NUMBER(channels, >, -1);
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

    #undef ParseOptions_DUPCHECK
    #undef ParseOptions_NEXTARG
    #undef ParseOptions_GETFLAG
    #undef ParseOptions_GETNUMBER
    #undef ParseOptions_GETPATH
    #undef ParseOptions_FLAG
    #undef ParseOptions_NUMBER
    #undef ParseOptions_PATH
}


int main(int argc, char *argv[]) {
    Options cmd_line_opts;
    int script_args_start;
    int po_ret = ParseOptions(argc, argv, &cmd_line_opts, &script_args_start);
    switch (po_ret) {
    case 1:
        return 1;
    case 2:
        return 0;
    default:
        break;
    }
    if (cmd_line_opts.has_script) { printf("script: %s\n", cmd_line_opts.script); }
    if (cmd_line_opts.has_config) { printf("config: %s\n", cmd_line_opts.config); }
    if (cmd_line_opts.has_base) { printf("base: %s\n", cmd_line_opts.base); }
    if (cmd_line_opts.has_width) { printf("width: %d\n", cmd_line_opts.width); }
    if (cmd_line_opts.has_height) { printf("height: %d\n", cmd_line_opts.height); }
    if (cmd_line_opts.has_fullscreen) { printf("fullscreen: %d\n", cmd_line_opts.fullscreen); }
    if (cmd_line_opts.has_channels) { printf("channels: %d\n", cmd_line_opts.channels); }
    if (script_args_start < argc) {
        printf("script args:");
        for (int i = script_args_start; i < argc; ++i) {
            printf(" %s", argv[i]);
        }
        printf("\n");
    }
    return 0;
}
