#include <lua.h>
#include <lauxlib.h>

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



/**
 * Declarations
 */

typedef struct {
    pid_t pid;
} spawn_Process;



/**
 * Exported spawn functions
 */

/* Create a new process, running the program of the current process. */
/* -> userdatum<spawn.process> */
static int ProcessRerun(lua_State *L) {
    spawn_Process *p;
    pid_t pid = fork();

    switch (pid) {
    case -1:
        return luaL_error(L, "fork(): %s", strerror(errno));

    case 0:
        /* Child process. Rerun this program. */
        execl("/proc/self/exe", "/proc/self/exe", (char *)0);
        /* If exec returns anything, an error occurred. */
        return luaL_error(L, "execl: %s", strerror(errno));

    default:
        /* Parent process. Prepare and return the userdatum. */
        p = (spawn_Process *)lua_newuserdata(L, sizeof(spawn_Process));
        p->pid = pid;
        break;
    }

    return 1;
}



/**
 * Public API
 */

static const struct luaL_reg spawn_ProcessMethods[] = {
    {"rerun", ProcessRerun},
    {NULL, NULL}
};

int luaopen_spawn(lua_State *L) {
    /* Don't require wait() for child processes. */
    signal(SIGCHLD, SIG_IGN);

    lua_createtable(L, 0, 1);

    lua_pushliteral(L, "process");
    luaL_newmetatable(L, "spawn.process");
    luaL_register(L, NULL, spawn_ProcessMethods);
    lua_rawset(L, -3);

    lua_setglobal(L, "spawn");
    lua_getglobal(L, "spawn");
    return 1;
}
