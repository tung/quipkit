#include <stdio.h>

#include "options.h"


int main(int argc, char *argv[]) {
    opt_Options cmd_line_opts;
    int script_args_start;
    int po_ret = opt_ParseOptions(argc, argv, &cmd_line_opts, &script_args_start);
    switch (po_ret) {
        case 1: return 1;
        case 2: return 0;
        default: break;
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
