#include <SDL.h>
#include <lua.h>
#include <lauxlib.h>

#include "event.h"
#include "name_Uint_pair.h"



typedef struct {
    const char *name;
    SDLKey key;
} name_SDLKey_pair;

typedef struct {
    const char *name;
    SDLMod mod;
} name_SDLMod_pair;



/* Deflate joystick hat positions into an array. Access like event.jhat.value[SDL.HAT_CENTERED]. */
/* Not sure how to incorporate SDL's convenience macros. */
static void hat_position_to_table(lua_State *L, Uint8 value) {
    lua_newtable(L);
    #define sethat(pos) \
        do { \
            lua_pushboolean(L, value & SDL_HAT_##pos); \
            lua_rawseti(L, -2, SDL_HAT_##pos); \
        } while (0)
    sethat(CENTERED);
    sethat(UP);
    sethat(RIGHT);
    sethat(DOWN);
    sethat(LEFT);
    #undef sethat
}


/* Inflate Lua table at index into hat position bitmask. */
static Uint8 table_to_hat_position(lua_State *L, int index) {
    Uint8 hat_pos = 0;
    #define gethat(pos) \
        do { \
            lua_pushinteger(L, SDL_HAT_##pos); \
            lua_gettable(L, index < 0 ? index - 1 : index); \
            if (lua_toboolean(L, -1)) { \
                hat_pos |= SDL_HAT_##pos; \
            } \
            lua_pop(L, 1); \
        } while (0)
    gethat(CENTERED);
    gethat(UP);
    gethat(RIGHT);
    gethat(DOWN);
    gethat(LEFT);
    #undef gethat
    return hat_pos;
}


/* Deflate key mods into an array. Access like event.key.keysym.mod[SDL.LSHIFT]. */
/* Not sure how to incorporate SDL's convenience macros. */
static void mod_to_table(lua_State *L, SDLMod mod) {
    lua_newtable(L);
    #define setmod(m) \
        do { \
            lua_pushboolean(L, mod & KMOD_##m); \
            lua_rawseti(L, -2, KMOD_##m); \
        } while (0)
    setmod(LSHIFT);
    setmod(RSHIFT);
    setmod(LCTRL);
    setmod(RCTRL);
    setmod(LALT);
    setmod(RALT);
    setmod(LMETA);
    setmod(RMETA);
    setmod(NUM);
    setmod(CAPS);
    setmod(MODE);
    #undef setmod
}


static SDLMod table_to_mod(lua_State *L, int index) {
    SDLMod mod = 0;
    #define getmod(m) \
        do { \
            lua_pushinteger(L, KMOD_##m); \
            lua_gettable(L, index < 0 ? index - 1 : index); \
            if (lua_toboolean(L, -1)) { \
                mod |= KMOD_##m; \
            } \
            lua_pop(L, 1); \
        } while (0)
    getmod(LSHIFT);
    getmod(RSHIFT);
    getmod(LCTRL);
    getmod(RCTRL);
    getmod(LALT);
    getmod(RALT);
    getmod(LMETA);
    getmod(RMETA);
    getmod(NUM);
    getmod(CAPS);
    getmod(MODE);
    #undef getmod
    return mod;
}


static void unicode_to_string(lua_State *L, Uint16 uc_char) {
    char buf[] = "\0\0";
    /* I don't know what to choose here, so I'm going with big endian. */
    buf[0] = (uc_char & 0xff00) << 8;
    buf[1] = uc_char & 0x00ff;
    lua_pushlstring(L, buf, 2);
}


static Uint16 string_to_unicode(lua_State *L, int index) {
    Uint16 uc_char;
    size_t l;
    const char *s = lua_tolstring(L, index, &l);
    /* Go with big endian, as above. */
    uc_char = 0;
    if (l >= 1) {
        uc_char = s[1] << 8;
        if (l >= 2) {
            uc_char |= s[0];
        }
    }
    return uc_char;
}


static void keysym_to_table(lua_State *L, SDL_keysym keysym) {
    lua_createtable(L, 0, 4);

    lua_pushliteral(L, "scancode");
    lua_pushinteger(L, keysym.scancode);
    lua_rawset(L, -3);

    lua_pushliteral(L, "sym");
    lua_pushinteger(L, keysym.sym);
    lua_rawset(L, -3);

    lua_pushliteral(L, "mod");
    mod_to_table(L, keysym.mod);
    lua_rawset(L, -3);

    lua_pushliteral(L, "unicode");
    unicode_to_string(L, keysym.unicode);
    lua_rawset(L, -3);
}


static void table_to_keysym(lua_State *L, int index, SDL_Event *event) {
    lua_pushliteral(L, "scancode");
    lua_gettable(L, index < 0 ? index - 1: index);
    event->key.keysym.scancode = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_pushliteral(L, "sym");
    lua_gettable(L, index < 0 ? index - 1: index);
    event->key.keysym.sym = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_pushliteral(L, "mod");
    lua_gettable(L, index < 0 ? index - 1: index);
    event->key.keysym.mod = table_to_mod(L, -1);
    lua_pop(L, 1);

    lua_pushliteral(L, "unicode");
    lua_gettable(L, index < 0 ? index - 1: index);
    switch (lua_type(L, -1)) {
        case LUA_TNIL:
            event->key.keysym.unicode = 0;
            break;
        case LUA_TNUMBER:
            event->key.keysym.unicode = lua_tointeger(L, -1);
            break;
        case LUA_TSTRING:
            event->key.keysym.unicode = string_to_unicode(L, -1);
            break;
        default:
            luaL_error(L, "bad unicode field type (expected nil/number/string, got %s)", lua_typename(L, -1));
    }
    lua_pop(L, 1);
}


/* Deflate bitmask into an array. Access like event.motion.state[1] for mouse button 1. */
static void button_state_to_table(lua_State *L, Uint8 state) {
    lua_createtable(L, 8, 0);
    lua_pushboolean(L, state & 0x01); lua_rawseti(L, -2, 1);
    lua_pushboolean(L, state & 0x02); lua_rawseti(L, -2, 2);
    lua_pushboolean(L, state & 0x04); lua_rawseti(L, -2, 3);
    lua_pushboolean(L, state & 0x08); lua_rawseti(L, -2, 4);
    lua_pushboolean(L, state & 0x10); lua_rawseti(L, -2, 5);
    lua_pushboolean(L, state & 0x20); lua_rawseti(L, -2, 6);
    lua_pushboolean(L, state & 0x40); lua_rawseti(L, -2, 7);
    lua_pushboolean(L, state & 0x80); lua_rawseti(L, -2, 8);
}


static Uint8 table_to_button_state(lua_State *L, int index) {
    Uint8 state = 0;
    #define getstateflag(n) \
        do { \
            lua_pushinteger(L, n); \
            lua_gettable(L, index < 0 ? index - 1 : index); \
            if (lua_toboolean(L, -1)) { \
                state |= 1 << (8 - n); \
            } \
            lua_pop(L, 1); \
        } while (0)
    getstateflag(1);
    getstateflag(2);
    getstateflag(3);
    getstateflag(4);
    getstateflag(5);
    getstateflag(6);
    getstateflag(7);
    getstateflag(8);
    #undef getstateflag
    return state;
}


/* Convert an SDL event into a Lua table. Helper function. */
static void event_to_table(lua_State *L, SDL_Event event) {
    lua_createtable(L, 0, 2);

    #define setint(t, n) \
        do { \
            lua_pushliteral(L, #n); \
            lua_pushinteger(L, event.t.n); \
            lua_rawset(L, -3); \
        } while (0)

    lua_pushliteral(L, "type");
    lua_pushinteger(L, event.type);
    lua_rawset(L, -3);

    switch (event.type) {
        case SDL_ACTIVEEVENT:
            lua_pushliteral(L, "active");
            lua_createtable(L, 0, 3);

            /* SDL_ACTIVEEVENT */
            setint(active, type);

            /* 0 for loss, 1 for gain */
            setint(active, gain);

            /* SDL_APPMOUSEFOCUS, SDL_APPINPUTFOCUS or SDL_APPACTIVE */
            setint(active, state);

            lua_rawset(L, -3);
            break;

        case SDL_JOYAXISMOTION:
            lua_pushliteral(L, "jaxis");
            lua_createtable(L, 0, 4);

            /* SDL_JOYAXISMOTION */
            setint(jaxis, type);

            setint(jaxis, which);
            setint(jaxis, axis);
            setint(jaxis, value);

            lua_rawset(L, -3);
            break;

        case SDL_JOYBALLMOTION:
            lua_pushliteral(L, "jball");
            lua_createtable(L, 0, 5);

            /* SDL_JOYBALLMOTION */
            setint(jball, type);

            setint(jball, which);
            setint(jball, ball);
            setint(jball, xrel);
            setint(jball, yrel);

            lua_rawset(L, -3);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            lua_pushliteral(L, "jbutton");
            lua_createtable(L, 0, 4);

            /* SDL_JOYBUTTONDOWN/UP */
            setint(jbutton, type);

            setint(jbutton, which);
            setint(jbutton, button);

            /* SDL_PRESSED/RELEASED */
            setint(jbutton, state);

            lua_rawset(L, -3);
            break;

        case SDL_JOYHATMOTION:
            lua_pushliteral(L, "jhat");
            lua_createtable(L, 0, 4);

            /* SDL_JOYHATMOTION */
            setint(jhat, type);

            setint(jhat, which);
            setint(jhat, hat);

            lua_pushliteral(L, "value");
            hat_position_to_table(L, event.jhat.value);
            lua_rawset(L, -3);

            lua_rawset(L, -3);
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            lua_pushliteral(L, "key");
            lua_createtable(L, 0, 3);

            /* SDL_KEYDOWN/UP */
            setint(key, type);

            /* SDL_PRESSED/RELEASED */
            setint(key, state);

            lua_pushliteral(L, "keysym");
            keysym_to_table(L, event.key.keysym);
            lua_rawset(L, -3);

            lua_rawset(L, -3);
            break;

        case SDL_MOUSEMOTION:
            lua_pushliteral(L, "motion");
            lua_createtable(L, 0, 6);

            /* SDL_MOUSEMOTION */
            setint(motion, type);

            lua_pushliteral(L, "state");
            button_state_to_table(L, event.motion.state);
            lua_rawset(L, -3);

            setint(motion, x);
            setint(motion, y);
            setint(motion, xrel);
            setint(motion, yrel);

            lua_rawset(L, -3);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            lua_pushliteral(L, "button");
            lua_createtable(L, 0, 5);

            /* SDL_MOUSEBUTTONDOWN/UP */
            setint(button, type);

            /* SDL_BUTTON_LEFT/MIDDLE/RIGHT */
            setint(button, button);

            /* SDL_PRESSED or SDL_RELEASED */
            setint(button, state);

            setint(button, x);
            setint(button, y);

            lua_rawset(L, -3);
            break;

        case SDL_QUIT:
            lua_pushliteral(L, "quit");
            lua_createtable(L, 0, 1);

            /* SDL_QUIT */
            setint(quit, type);

            lua_rawset(L, -3);
            break;

        /* SDL_USEREVENT through to (SDL_NUMEVENTS - 1) are handled in the default case. */

        case SDL_VIDEOEXPOSE:
            lua_pushliteral(L, "expose");
            lua_createtable(L, 0, 1);

            /* SDL_VIDEOEXPOSE */
            setint(expose, type);

            lua_rawset(L, -3);
            break;

        case SDL_VIDEORESIZE:
            lua_pushliteral(L, "resize");
            lua_createtable(L, 0, 3);

            /* SDL_VIDEORESIZE */
            setint(resize, type);

            setint(resize, w);
            setint(resize, h);

            lua_rawset(L, -3);
            break;

        default:
            if (event.type >= SDL_USEREVENT && event.type < SDL_NUMEVENTS) {
                lua_pushliteral(L, "user");
                lua_createtable(L, 0, 4);

                /* SDL_USEREVENT through to (SDL_NUMEVENTS - 1) */
                setint(user, type);

                setint(user, code);

                /* Works as long as sizeof(Lua ref int) <= sizeof(void *) */
                lua_pushliteral(L, "data1");
                lua_rawgeti(L, LUA_REGISTRYINDEX, (int)event.user.data1);
                lua_rawset(L, -3);

                /* ^ ditto */
                lua_pushliteral(L, "data2");
                lua_rawgeti(L, LUA_REGISTRYINDEX, (int)event.user.data2);
                lua_rawset(L, -3);

                lua_rawset(L, -3);
            } else {
                luaL_error(L, "could not convert unrecognized event (%d)", event.type);
            }
    }
    #undef setint
}


static void free_userevent_data(lua_State *L, SDL_Event user_event) {
    luaL_unref(L, LUA_REGISTRYINDEX, (int)user_event.user.data1);
    luaL_unref(L, LUA_REGISTRYINDEX, (int)user_event.user.data2);
}



/* number key -> string key_name */
static int GetKeyName(lua_State *L) {
    SDLKey key = luaL_checkint(L, 1);
    const char *key_name = SDL_GetKeyName(key);
    lua_pushstring(L, key_name);
    return 1;
}


/* -> array buttons_state{0..8}, number x, number y */
static int GetMouseState(lua_State *L) {
    int x, y;
    Uint8 buttons = SDL_GetMouseState(&x, &y);
    button_state_to_table(L, buttons);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 3;
}


/* -> (nil | table event) */
static int PollEvent(lua_State *L) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        event_to_table(L, event);
        if (event.type >= SDL_USEREVENT && event.type < SDL_NUMEVENTS) {
            free_userevent_data(L, event);
        }
    } else {
        lua_pushnil(L);
    }
    return 1;
}


/* -> */
static int PumpEvents(lua_State *L) {
    (void)L;
    SDL_PumpEvents();
    return 0;
}


/* table event -> */
static int PushEvent(lua_State *L) {
    SDL_Event event;
    int type;

    luaL_checktype(L, 1, LUA_TTABLE);

    /* Get the event type. */
    lua_pushliteral(L, "type");
    lua_gettable(L, 1);
    type = lua_tointeger(L, -1);
    event.type = type;

    #define pushevent(st, n) \
        do { \
            event.n.type = type; \
            lua_pushliteral(st, #n); \
            lua_gettable(st, 1); \
        } while (0)

    #define getint(t, n) \
        do { \
            lua_pushliteral(L, #n); \
            lua_gettable(L, -2); \
            event.t.n = lua_tointeger(L, -1); \
            lua_pop(L, 1); \
        } while (0)

    #define popevent(st) \
        do { \
            lua_pop(st, 1); \
        } while (0)

    switch (event.type) {
        case SDL_ACTIVEEVENT:
            pushevent(L, active);
            getint(active, gain);
            getint(active, state);
            popevent(L);
            break;

        case SDL_JOYAXISMOTION:
            pushevent(L, jaxis);
            getint(jaxis, which);
            getint(jaxis, axis);
            getint(jaxis, value);
            popevent(L);
            break;

        case SDL_JOYBALLMOTION:
            pushevent(L, jball);
            getint(jball, which);
            getint(jball, ball);
            getint(jball, xrel);
            getint(jball, yrel);
            popevent(L);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            pushevent(L, jbutton);
            getint(jbutton, which);
            getint(jbutton, button);
            getint(jbutton, state);
            popevent(L);
            break;

        case SDL_JOYHATMOTION:
            pushevent(L, jhat);
            getint(jhat, which);
            getint(jhat, hat);

            lua_pushliteral(L, "value");
            lua_gettable(L, -2);
            event.jhat.value = table_to_hat_position(L, -1);
            lua_pop(L, 1);

            popevent(L);
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            pushevent(L, key);
            getint(key, state);

            lua_pushliteral(L, "keysym");
            lua_gettable(L, -2);
            table_to_keysym(L, -1, &event);
            lua_pop(L, 1);

            popevent(L);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            pushevent(L, button);
            getint(button, button);
            getint(button, state);
            getint(button, x);
            getint(button, y);
            popevent(L);
            break;

        case SDL_MOUSEMOTION:
            pushevent(L, motion);

            lua_pushliteral(L, "state");
            lua_gettable(L, -2);
            event.motion.state = table_to_button_state(L, -1);
            lua_pop(L, 1);

            getint(motion, x);
            getint(motion, y);
            getint(motion, xrel);
            getint(motion, yrel);
            popevent(L);
            break;

        case SDL_QUIT:
            event.quit.type = type;
            break;

        /* SDL_SYSWMEVENT intentionally omitted. Too platform-specific, not enough gain. */
        /* SDL_USEREVENT through (SDL_NUMEVENTS - 1) handled in default case. */

        case SDL_VIDEOEXPOSE:
            event.expose.type = type;
            break;

        case SDL_VIDEORESIZE:
            pushevent(L, resize);
            getint(resize, w);
            getint(resize, h);
            popevent(L);
            break;

        default:
            if (type >= SDL_USEREVENT && type < SDL_NUMEVENTS) {
                pushevent(L, user);
                getint(user, code);

                /* Works as long as sizeof(Lua ref int) <= sizeof(void *). */
                lua_pushliteral(L, "data1");
                lua_gettable(L, -2);
                event.user.data1 = (void *)luaL_ref(L, LUA_REGISTRYINDEX);

                /* ^ ditto */
                lua_pushliteral(L, "data2");
                lua_gettable(L, -2);
                event.user.data2 = (void *)luaL_ref(L, LUA_REGISTRYINDEX);

                popevent(L);
            } else {
                luaL_error(L, "could not push unrecognized event (%d)", type);
            }
            break;
    }

    #undef pushevent
    #undef getint
    #undef popevent

    if (SDL_PushEvent(&event)) {
        luaL_error(L, "SDL_PushEvent failed: %s", SDL_GetError());
    }

    return 0;
}


/* -> table event */
static int WaitEvent(lua_State *L) {
    SDL_Event event;
    if (SDL_WaitEvent(&event) == 0) {
        luaL_error(L, "SDL_WaitEvent failed: %s", SDL_GetError());
    }
    event_to_table(L, event);
    if (event.type >= SDL_USEREVENT && event.type < SDL_NUMEVENTS) {
        free_userevent_data(L, event);
    }
    return 1;
}



#define defeconst(name) {#name, SDL_##name}
static const name_Uint8_pair sdl_event_constants[] = {
    /* SDL event constants */
    defeconst(ACTIVEEVENT),
    defeconst(JOYAXISMOTION),
    defeconst(JOYBALLMOTION),
    defeconst(JOYBUTTONDOWN),
    defeconst(JOYBUTTONUP),
    defeconst(JOYHATMOTION),
    defeconst(KEYDOWN),
    defeconst(KEYUP),
    defeconst(MOUSEBUTTONDOWN),
    defeconst(MOUSEBUTTONUP),
    defeconst(MOUSEMOTION),
    defeconst(QUIT),
    /* SDL_SYSWMEVENT omitted due to massive cross-platform effort needed. */
    defeconst(USEREVENT),
    defeconst(VIDEOEXPOSE),
    defeconst(VIDEORESIZE),
    defeconst(NUMEVENTS),

    /* SDL_ACTIVEEVENT state constants */
    defeconst(APPMOUSEFOCUS),
    defeconst(APPINPUTFOCUS),
    defeconst(APPACTIVE),

    /* mouse button constants */
    defeconst(BUTTON_LEFT),
    defeconst(BUTTON_MIDDLE),
    defeconst(BUTTON_RIGHT),

    /* press state constants */
    defeconst(PRESSED),
    defeconst(RELEASED),

    {NULL, 0}
};
#undef defeconst

/* Load SDL event constants into SDL module table at index. */
static void load_sdl_event_constants(lua_State *L, int index) {
    const name_Uint8_pair *p;
    for (p = sdl_event_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->uint);
        lua_settable(L, index < 0 ? index - 2 : index);
    }
}


#define defkey(name) {#name, SDLK_##name}
static const name_SDLKey_pair sdl_key_constants[] = {
    defkey(BACKSPACE),
    defkey(TAB),
    defkey(CLEAR),
    defkey(RETURN),
    defkey(PAUSE),
    defkey(ESCAPE),
    defkey(SPACE),
    defkey(EXCLAIM),
    defkey(QUOTEDBL),
    defkey(HASH),
    defkey(DOLLAR),
    defkey(AMPERSAND),
    defkey(QUOTE),
    defkey(LEFTPAREN),
    defkey(RIGHTPAREN),
    defkey(ASTERISK),
    defkey(PLUS),
    defkey(COMMA),
    defkey(MINUS),
    defkey(PERIOD),
    defkey(SLASH),
    defkey(0),
    defkey(1),
    defkey(2),
    defkey(3),
    defkey(4),
    defkey(5),
    defkey(6),
    defkey(7),
    defkey(8),
    defkey(9),
    defkey(COLON),
    defkey(SEMICOLON),
    defkey(LESS),
    defkey(EQUALS),
    defkey(GREATER),
    defkey(QUESTION),
    defkey(AT),
    defkey(LEFTBRACKET),
    defkey(BACKSLASH),
    defkey(RIGHTBRACKET),
    defkey(CARET),
    defkey(UNDERSCORE),
    defkey(BACKQUOTE),
    defkey(a),
    defkey(b),
    defkey(c),
    defkey(d),
    defkey(e),
    defkey(f),
    defkey(g),
    defkey(h),
    defkey(i),
    defkey(j),
    defkey(k),
    defkey(l),
    defkey(m),
    defkey(n),
    defkey(o),
    defkey(p),
    defkey(q),
    defkey(r),
    defkey(s),
    defkey(t),
    defkey(u),
    defkey(v),
    defkey(w),
    defkey(x),
    defkey(y),
    defkey(z),
    defkey(DELETE),
    defkey(KP0),
    defkey(KP1),
    defkey(KP2),
    defkey(KP3),
    defkey(KP4),
    defkey(KP5),
    defkey(KP6),
    defkey(KP7),
    defkey(KP8),
    defkey(KP9),
    defkey(KP_PERIOD),
    defkey(KP_DIVIDE),
    defkey(KP_MULTIPLY),
    defkey(KP_MINUS),
    defkey(KP_PLUS),
    defkey(KP_ENTER),
    defkey(KP_EQUALS),
    defkey(UP),
    defkey(DOWN),
    defkey(RIGHT),
    defkey(LEFT),
    defkey(INSERT),
    defkey(HOME),
    defkey(END),
    defkey(PAGEUP),
    defkey(PAGEDOWN),
    defkey(F1),
    defkey(F2),
    defkey(F3),
    defkey(F4),
    defkey(F5),
    defkey(F6),
    defkey(F7),
    defkey(F8),
    defkey(F9),
    defkey(F10),
    defkey(F11),
    defkey(F12),
    defkey(F13),
    defkey(F14),
    defkey(F15),
    defkey(NUMLOCK),
    defkey(CAPSLOCK),
    defkey(SCROLLOCK),
    defkey(RSHIFT),
    defkey(LSHIFT),
    defkey(RCTRL),
    defkey(LCTRL),
    defkey(RALT),
    defkey(LALT),
    defkey(RMETA),
    defkey(LMETA),
    defkey(LSUPER),
    defkey(RSUPER),
    defkey(MODE),
    defkey(HELP),
    defkey(PRINT),
    defkey(SYSREQ),
    defkey(BREAK),
    defkey(MENU),
    defkey(POWER),
    defkey(EURO),
    {NULL, 0}
};
#undef defkey

/* Load SDL key constants into SDL module table at index. */
static void load_sdlk_constants(lua_State *L, int index) {
    const name_SDLKey_pair *p;
    lua_createtable(L, 0, sizeof(sdl_key_constants) / sizeof(name_SDLKey_pair));
    for (p = sdl_key_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->key);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
    lua_setglobal(L, "SDLK");
}


#define defkmod(name) {#name, KMOD_##name}
static const name_SDLMod_pair sdl_kmod_constants[] = {
    defkmod(LSHIFT),
    defkmod(RSHIFT),
    defkmod(LCTRL),
    defkmod(RCTRL),
    defkmod(LALT),
    defkmod(RALT),
    defkmod(LMETA),
    defkmod(RMETA),
    defkmod(NUM),
    defkmod(CAPS),
    defkmod(MODE),
    {NULL, 0}
};
#undef defkmod

/* Load SDL key modifier constants into SDL module table at index. */
static void load_kmod_constants(lua_State *L, int index) {
    const name_SDLMod_pair *p;
    lua_createtable(L, 0, sizeof(sdl_kmod_constants) / sizeof(name_SDLMod_pair));
    for (p = sdl_kmod_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->mod);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
    lua_setglobal(L, "KMOD");
}


static const luaL_reg sdl_event_functions [] = {
    {"GetKeyName", GetKeyName},
    {"GetMouseState", GetMouseState},
    {"PollEvent", PollEvent},
    {"PumpEvents", PumpEvents},
    {"PushEvent", PushEvent},
    {"WaitEvent", WaitEvent},
    {NULL, NULL}
};

void load_sdl_event(lua_State *L, int index) {
    /* Assume "SDL" module table is on top of the stack. */
    luaL_register(L, NULL, sdl_event_functions);
    load_sdl_event_constants(L, index);
    load_sdlk_constants(L, index);
    load_kmod_constants(L, index);
}
