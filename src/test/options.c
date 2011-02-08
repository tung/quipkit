#include "../options.h"

#include <stdio.h>
#include <string.h>


#define PRINT_OPT_ERROR(poe_funcname) \
    fprintf(stderr, "%s:%d: " #poe_funcname " failed: %s\n", __FILE__, __LINE__, opt_GetError())


#define TEST_ARGS(test_num, ...) \
    char *argv[] = { __VA_ARGS__ }; \
    int argc = sizeof(argv) / sizeof(char *); \
    opt_Options o; \
    int script_args_start; \
    int po_ret = opt_ParseOptions(argc, argv, &o, &script_args_start); \
    if (po_ret) { \
        PRINT_OPT_ERROR(opt_ParseOptions); \
        tests_run += test_num; \
        tests_failed += test_num; \
        return; \
    }

#define TEST_ASSERT(cond) \
    do { \
        tests_run += 1; \
        if (!(cond)) { \
            fprintf(stderr, "%s:%d: " #cond " failed\n", __FILE__, __LINE__); \
            tests_failed += 1; \
        } \
    } while(0)


int tests_run;
int tests_failed;


static void TestPaths() {
    TEST_ARGS(6, "./options", "--script", "startup/main.lua", "--config", "startup/config.txt", "--base", "startup");
    (void)script_args_start;

    TEST_ASSERT(o.has_script);
    TEST_ASSERT(o.has_config);
    TEST_ASSERT(o.has_base);

    TEST_ASSERT(strstr(o.script, "startup/main.lua"));
    TEST_ASSERT(strstr(o.config, "startup/config.txt"));
    TEST_ASSERT(strstr(o.base, "startup"));
}


static void TestScreen() {
    TEST_ARGS(6, "./options", "--width", "1024", "--height", "768", "--fullscreen", "1");
    (void)script_args_start;

    TEST_ASSERT(o.has_width);
    TEST_ASSERT(o.has_height);
    TEST_ASSERT(o.has_fullscreen);

    TEST_ASSERT(o.width == 1024);
    TEST_ASSERT(o.height == 768);
    TEST_ASSERT(o.fullscreen == 1);
}


static void TestChannels() {
    TEST_ARGS(2, "./options", "--channels", "16");
    (void)script_args_start;
    TEST_ASSERT(o.has_channels);
    TEST_ASSERT(o.channels == 16);
}


static void TestScriptArgs() {
    TEST_ARGS(1, "./options", "--", "abc", "xyz");
    (void)o;
    TEST_ASSERT(script_args_start == 2);
}


static void TestShortForm() {
    TEST_ARGS(3, "./options", "startup/main.lua", "abc", "xyz");
    TEST_ASSERT(o.has_script);
    TEST_ASSERT(strstr(o.script, "startup/main.lua"));
    TEST_ASSERT(script_args_start == 2);
}


static void TestShortFormBad() {
    char *argv[] = { "./options", "i_dont_exist", "abc", "xyz" };
    int argc = sizeof(argv) / sizeof(char *);
    opt_Options o;
    int script_args_start;
    int po_ret = opt_ParseOptions(argc, argv, &o, &script_args_start);
    TEST_ASSERT(po_ret != 0);
}


static void TestMixedArgs() {
    TEST_ARGS(7, "./options", "--width", "800", "--height", "600", "--script", "startup/main.lua", "--", "abc", "xyz");

    TEST_ASSERT(o.has_width);
    TEST_ASSERT(o.has_height);
    TEST_ASSERT(o.has_script);

    TEST_ASSERT(script_args_start == 8);

    TEST_ASSERT(o.width == 800);
    TEST_ASSERT(o.height == 600);
    TEST_ASSERT(strstr(o.script, "startup/main.lua"));
}


static void TestConfigGameDefaults() {
    TEST_ARGS(12, "./options");

    opt_Options c;
    c.has_script = 0;
    c.has_config = 0;
    c.has_base = 0;
    c.has_width = 0;
    c.has_height = 0;
    c.has_fullscreen = 0;
    c.has_channels = 0;

    opt_Options g;
    if (opt_ConfigGame(&o, &c, &g)) {
        PRINT_OPT_ERROR(opt_ConfigGame);
        tests_run += 12;
        tests_failed += 12;
        return;
    }

    TEST_ASSERT(g.has_script);
    TEST_ASSERT(g.has_base);
    TEST_ASSERT(g.has_width);
    TEST_ASSERT(g.has_height);
    TEST_ASSERT(g.has_fullscreen);
    TEST_ASSERT(g.has_channels);

    TEST_ASSERT(strstr(g.script, "startup/main.lua"));
    TEST_ASSERT(strstr(g.base, "startup"));
    TEST_ASSERT(g.width == DEFAULT_WIDTH);
    TEST_ASSERT(g.height == DEFAULT_HEIGHT);
    TEST_ASSERT(g.fullscreen == DEFAULT_FULLSCREEN);
    TEST_ASSERT(g.channels == DEFAULT_CHANNELS);
}


static void TestConfigGameCommandLineOnly() {
    TEST_ARGS(6, "./options", "--script", "startup/main.lua", "--config", "startup/config.txt", "--base", "startup",
            "--width", "123", "--height", "456", "--fullscreen", "1", "--channels", "8");

    opt_Options c;
    c.has_script = 0;
    c.has_config = 0;
    c.has_base = 0;
    c.has_width = 0;
    c.has_height = 0;
    c.has_fullscreen = 0;
    c.has_channels = 0;

    opt_Options g;
    if (opt_ConfigGame(&o, &c, &g)) {
        PRINT_OPT_ERROR(opt_ConfigGame);
        tests_run += 6;
        tests_failed += 6;
        return;
    }

    TEST_ASSERT(strstr(g.script, "startup/main.lua"));
    TEST_ASSERT(strstr(g.base, "startup"));
    TEST_ASSERT(g.width == 123);
    TEST_ASSERT(g.height == 456);
    TEST_ASSERT(g.fullscreen == 1);
    TEST_ASSERT(g.channels == 8);
}


static void TestConfigGameConfigOnly() {
    TEST_ARGS(5, "./options", "--config", "startup/config.txt");

    opt_Options c;
    if (opt_LoadConfig(&o, &c)) {
        PRINT_OPT_ERROR(opt_LoadConfig);
        tests_run += 5;
        tests_failed += 5;
        return;
    }

    opt_Options g;
    if (opt_ConfigGame(&o, &c, &g)) {
        PRINT_OPT_ERROR(opt_ConfigGame);
        tests_run += 5;
        tests_failed += 5;
        return;
    }

    /* Note: These values depend on what's in startup/config.txt. */
    TEST_ASSERT(strstr(g.base, "startup"));
    TEST_ASSERT(g.width == 800);
    TEST_ASSERT(g.height == 600);
    TEST_ASSERT(g.fullscreen == 0);
    TEST_ASSERT(g.channels == 16);
}


static void TestConfigGameAll() {
    TEST_ARGS(6, "./options", "--script", "startup/main.lua", "--config", "startup/config.txt", "--base", "startup",
            "--width", "123", "--height", "456", "--fullscreen", "1", "--channels", "8");

    opt_Options c;
    if (opt_LoadConfig(&o, &c)) {
        PRINT_OPT_ERROR(opt_LoadConfig);
        tests_run += 6;
        tests_failed += 6;
        return;
    }

    opt_Options g;
    if (opt_ConfigGame(&o, &c, &g)) {
        PRINT_OPT_ERROR(opt_ConfigGame);
        tests_run += 6;
        tests_failed += 6;
        return;
    }

    TEST_ASSERT(strstr(g.script, "startup/main.lua"));
    TEST_ASSERT(strstr(g.base, "startup"));
    TEST_ASSERT(g.width == 123);
    TEST_ASSERT(g.height == 456);
    TEST_ASSERT(g.fullscreen == 1);
    TEST_ASSERT(g.channels == 8);
}


static void TestConfigGameMixed() {
    TEST_ARGS(4, "./options", "--config", "startup/config.txt", "--base", "startup", "--width", "123", "--height", "456");

    opt_Options c;
    if (opt_LoadConfig(&o, &c)) {
        PRINT_OPT_ERROR(opt_LoadConfig);
        tests_run += 4;
        tests_failed += 4;
        return;
    }

    opt_Options g;
    if (opt_ConfigGame(&o, &c, &g)) {
        PRINT_OPT_ERROR(opt_ConfigGame);
        tests_run += 4;
        tests_failed += 4;
        return;
    }

    /* These come from the command line args... */
    TEST_ASSERT(g.width == 123);
    TEST_ASSERT(g.height == 456);
    /* ... and these come from startup/config.txt. */
    TEST_ASSERT(g.fullscreen == 0);
    TEST_ASSERT(g.channels == 16);
}


static void TestConfigGameMixed2() {
    TEST_ARGS(4, "./options", "--config", "startup/config.txt", "--base", "startup", "--fullscreen", "1", "--channels", "8");

    opt_Options c;
    if (opt_LoadConfig(&o, &c)) {
        PRINT_OPT_ERROR(opt_LoadConfig);
        tests_run += 4;
        tests_failed += 4;
        return;
    }

    opt_Options g;
    if (opt_ConfigGame(&o, &c, &g)) {
        PRINT_OPT_ERROR(opt_ConfigGame);
        tests_run += 4;
        tests_failed += 4;
        return;
    }

    /* These come from startup/config.txt... */
    TEST_ASSERT(g.width == 800);
    TEST_ASSERT(g.height == 600);
    /* ... and these come from the command line args. */
    TEST_ASSERT(g.fullscreen == 1);
    TEST_ASSERT(g.channels == 8);
}


int main() {
    tests_run = 0;
    tests_failed = 0;

    TestPaths();
    TestScreen();
    TestChannels();
    TestScriptArgs();
    TestShortForm();
    TestShortFormBad();
    TestMixedArgs();
    TestConfigGameDefaults();
    TestConfigGameCommandLineOnly();
    TestConfigGameConfigOnly();
    TestConfigGameAll();
    TestConfigGameMixed();
    TestConfigGameMixed2();

    printf("%d tests failed; %d tests run\n", tests_failed, tests_run);

    return tests_failed > 0 ? 1 : 0;
}
