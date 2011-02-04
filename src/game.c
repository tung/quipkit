#include <stdio.h>

#include "options.h"


static void dump_options(const char *label, const opt_Options *opts) {
    printf("%s:\n", label);
    if (opts->has_script) { printf("script: %s\n", opts->script); }
    if (opts->has_config) { printf("config: %s\n", opts->config); }
    if (opts->has_base) { printf("base: %s\n", opts->base); }
    if (opts->has_width) { printf("width: %d\n", opts->width); }
    if (opts->has_height) { printf("height: %d\n", opts->height); }
    if (opts->has_fullscreen) { printf("fullscreen: %d\n", opts->fullscreen); }
    if (opts->has_channels) { printf("channels: %d\n", opts->channels); }
}


int main(int argc, char *argv[]) {
    opt_Options cmd_line_opts, config_opts;
    int script_args_start;
    int po_ret = opt_ParseOptions(argc, argv, &cmd_line_opts, &script_args_start);
    switch (po_ret) {
        case 1: return 1;
        case 2: return 0;
        default: break;
    }
    dump_options("Command line options", &cmd_line_opts);

    int lc_ret = opt_LoadConfig(&cmd_line_opts, &config_opts);
    switch (lc_ret) {
        case 1: fprintf(stderr, "couldn't init config reader Lua state\n"); return 1;
        case 2: fprintf(stderr, "conf.lua missing\n"); return 1;
        case 3: fprintf(stderr, "config file missing or error in config file\n"); return 1;
        case 4: fprintf(stderr, "'base' path not found\n"); return 1;
        case 5: fprintf(stderr, "'base' not a string\n"); return 1;
        case 6: fprintf(stderr, "'width' <= 0\n"); return 1;
        case 7: fprintf(stderr, "'width' not a number"); return 1;
        case 8: fprintf(stderr, "'height' <= 0\n"); return 1;
        case 9: fprintf(stderr, "'height' not a number"); return 1;
        case 10: fprintf(stderr, "'fullscreen' was a number other than 0 (off) or 1 (on)\n"); return 1;
        case 11: fprintf(stderr, "'fullscreen' not a number of boolean\n"); return 1;
        case 12: fprintf(stderr, "'channels' < 0"); return 1;
        case 13: fprintf(stderr, "'channels' not a number"); return 1;
        default: break;
    }
    dump_options("Config options", &config_opts);

    if (script_args_start < argc) {
        printf("Script args:");
        for (int i = script_args_start; i < argc; ++i) {
            printf(" %s", argv[i]);
        }
        printf("\n");
    }

    return 0;
}
