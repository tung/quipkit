#include "../options.h"

#include <stdio.h>
#include <string.h>


#define TEST_ARGS(test_num, ...) \
    char *argv[] = { __VA_ARGS__ }; \
    int argc = sizeof(argv) / sizeof(char *); \
    opt_Options o; \
    int script_args_start; \
    int po_ret = opt_ParseOptions(argc, argv, &o, &script_args_start); \
    switch (po_ret) { \
        case 1: \
        case 2: \
            tests_run = test_num; \
            tests_failed = test_num; \
            break; \
        default: \
            break; \
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


int main() {
    tests_run = 0;
    tests_failed = 0;

    TestPaths();
    TestScreen();
    TestChannels();
    TestScriptArgs();
    TestShortForm();
    TestMixedArgs();

    printf("%d tests failed; %d tests run\n", tests_failed, tests_run);

    return tests_failed > 0 ? 1 : 0;
}
