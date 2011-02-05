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
    if (po_ret) {
        printf("%s\n", opt_GetError());
        if (po_ret == -1) { return 0; }
        return 1;
    }
    dump_options("Command line options", &cmd_line_opts);

    int lc_ret = opt_LoadConfig(&cmd_line_opts, &config_opts);
    if (lc_ret) {
        printf("%s\n", opt_GetError());
        return 1;
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
