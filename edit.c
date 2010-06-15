#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>

const char *INIT_SCRIPT = "dev/init.lua";
const char *MAIN_SCRIPT = "dev/main.lua";



int main(int argc, char *argv[]) {
    lua_State *L = (lua_State *)lua_open();
    luaL_openlibs(L);

    if (luaL_loadfile(L, INIT_SCRIPT) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Init script failed: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    const char *main_script = MAIN_SCRIPT;
    if (argc > 1) {
        main_script = argv[1];
    }

    if (luaL_loadfile(L, main_script) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    lua_close(L);
    return 0;
}
