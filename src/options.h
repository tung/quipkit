#ifndef QK_OPTIONS_H
#define QK_OPTIONS_H


#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_FULLSCREEN 0
#define DEFAULT_CHANNELS 16

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
 * Get the message from the last failed function call.
 */
const char *opt_GetError();

/**
 * Read program arguments into opts, and mark where script args begin in script_args_start.
 * Returns 0 on success, 1 on failure and -1 to exit with a message.
 */
int opt_ParseOptions(int argc, char *argv[], opt_Options *opts, int *script_args_start);

/**
 * Load the game config file.
 * Returns 0 on success or 1 on failure.
 */
int opt_LoadConfig(const opt_Options *cmd_line_opts, opt_Options *config_opts);

/**
 * Configure game_opts with options derived from command line arguments and config file.
 */
int opt_ConfigGame(const opt_Options *cmd_line_opts, const opt_Options *config_opts, opt_Options *game_opts);


#endif
