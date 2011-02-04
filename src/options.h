#ifndef QK_OPTIONS_H
#define QK_OPTIONS_H


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
} opt_Options;


/**
 * Read program arguments into opts, and mark where script args begin in script_args_start.
 */
int opt_ParseOptions(int argc, char *argv[], opt_Options *opts, int *script_args_start);

/**
 * Load the game config file.
 */
int opt_LoadConfig(const opt_Options *cmd_line_opts, opt_Options *config_opts);


#endif
