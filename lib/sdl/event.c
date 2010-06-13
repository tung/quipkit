#include "event.h"

#include <lua.h>
#include <lauxlib.h>
#include <SDL.h>

#include "types.h"



/**
 * LuaSDL Support Functions
 */

/* Deflate joystick hat positions into an array. Access like event.jhat.value[SDL.HAT_CENTERED]. */
/* Not sure how to incorporate SDL's convenience macros. */
static void HatPositionToTable(lua_State *L, Uint8 value) {
    lua_newtable(L);
    #define SET_HAT(pos) \
        do { \
            lua_pushboolean(L, value & SDL_HAT_##pos); \
            lua_rawseti(L, -2, SDL_HAT_##pos); \
        } while (0)
    SET_HAT(CENTERED);
    SET_HAT(UP);
    SET_HAT(RIGHT);
    SET_HAT(DOWN);
    SET_HAT(LEFT);
    #undef SET_HAT
}


/* Inflate Lua table at index into hat position bitmask. */
static Uint8 TableToHatPosition(lua_State *L, int index) {
    Uint8 hat_pos = 0;
    #define GET_HAT(pos) \
        do { \
            lua_pushinteger(L, SDL_HAT_##pos); \
            lua_gettable(L, index < 0 ? index - 1 : index); \
            if (lua_toboolean(L, -1)) { \
                hat_pos |= SDL_HAT_##pos; \
            } \
            lua_pop(L, 1); \
        } while (0)
    GET_HAT(CENTERED);
    GET_HAT(UP);
    GET_HAT(RIGHT);
    GET_HAT(DOWN);
    GET_HAT(LEFT);
    #undef GET_HAT
    return hat_pos;
}


/* Deflate key mods into an array. Access like event.key.keysym.mod[SDL.LSHIFT]. */
/* Not sure how to incorporate SDL's convenience macros. */
static void ModToTable(lua_State *L, SDLMod mod) {
    lua_newtable(L);
    #define SET_MOD(m) \
        do { \
            lua_pushboolean(L, mod & KMOD_##m); \
            lua_rawseti(L, -2, KMOD_##m); \
        } while (0)
    SET_MOD(LSHIFT);
    SET_MOD(RSHIFT);
    SET_MOD(LCTRL);
    SET_MOD(RCTRL);
    SET_MOD(LALT);
    SET_MOD(RALT);
    SET_MOD(LMETA);
    SET_MOD(RMETA);
    SET_MOD(NUM);
    SET_MOD(CAPS);
    SET_MOD(MODE);
    #undef SET_MOD
}


static SDLMod TableToMod(lua_State *L, int index) {
    SDLMod mod = 0;
    #define GET_MOD(m) \
        do { \
            lua_pushinteger(L, KMOD_##m); \
            lua_gettable(L, index < 0 ? index - 1 : index); \
            if (lua_toboolean(L, -1)) { \
                mod |= KMOD_##m; \
            } \
            lua_pop(L, 1); \
        } while (0)
    GET_MOD(LSHIFT);
    GET_MOD(RSHIFT);
    GET_MOD(LCTRL);
    GET_MOD(RCTRL);
    GET_MOD(LALT);
    GET_MOD(RALT);
    GET_MOD(LMETA);
    GET_MOD(RMETA);
    GET_MOD(NUM);
    GET_MOD(CAPS);
    GET_MOD(MODE);
    #undef GET_MOD
    return mod;
}


static void UnicodeToString(lua_State *L, Uint16 uc_char) {
    char buf[] = "\0\0";
    /* I don't know what to choose here, so I'm going with big endian. */
    buf[0] = (uc_char & 0xff00) << 8;
    buf[1] = uc_char & 0x00ff;
    lua_pushlstring(L, buf, 2);
}


static Uint16 StringToUnicode(lua_State *L, int index) {
    size_t l;
    const char *s = lua_tolstring(L, index, &l);
    /* Go with big endian, as above. */
    Uint16 uc_char = 0;
    if (l >= 1) {
        uc_char = s[1] << 8;
        if (l >= 2) {
            uc_char |= s[0];
        }
    }
    return uc_char;
}


static void KeysymToTable(lua_State *L, SDL_keysym keysym) {
    lua_createtable(L, 0, 4);

    lua_pushliteral(L, "scancode");
    lua_pushinteger(L, keysym.scancode);
    lua_rawset(L, -3);

    lua_pushliteral(L, "sym");
    lua_pushinteger(L, keysym.sym);
    lua_rawset(L, -3);

    lua_pushliteral(L, "mod");
    ModToTable(L, keysym.mod);
    lua_rawset(L, -3);

    lua_pushliteral(L, "unicode");
    UnicodeToString(L, keysym.unicode);
    lua_rawset(L, -3);
}


static void TableToKeysym(lua_State *L, int index, SDL_Event *event) {
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
    event->key.keysym.mod = TableToMod(L, -1);
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
            event->key.keysym.unicode = StringToUnicode(L, -1);
            break;
        default:
            luaL_error(L, "bad unicode field type (expected nil/number/string, got %s)", lua_typename(L, -1));
    }
    lua_pop(L, 1);
}


/* Deflate bitmask into an array. Access like event.motion.state[1] for mouse button 1. */
static void ButtonStateToTable(lua_State *L, Uint8 state) {
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


static Uint8 TableToButtonState(lua_State *L, int index) {
    Uint8 state = 0;
    #define GET_STATE_FLAG(n) \
        do { \
            lua_pushinteger(L, n); \
            lua_gettable(L, index < 0 ? index - 1 : index); \
            if (lua_toboolean(L, -1)) { \
                state |= 1 << (8 - n); \
            } \
            lua_pop(L, 1); \
        } while (0)
    GET_STATE_FLAG(1);
    GET_STATE_FLAG(2);
    GET_STATE_FLAG(3);
    GET_STATE_FLAG(4);
    GET_STATE_FLAG(5);
    GET_STATE_FLAG(6);
    GET_STATE_FLAG(7);
    GET_STATE_FLAG(8);
    #undef GET_STATE_FLAG
    return state;
}


/* Convert an SDL event into a Lua table. Helper function. */
static void EventToTable(lua_State *L, SDL_Event event) {
    lua_createtable(L, 0, 2);

    #define SET_INT(t, n) \
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
            SET_INT(active, type);

            /* 0 for loss, 1 for gain */
            SET_INT(active, gain);

            /* SDL_APPMOUSEFOCUS, SDL_APPINPUTFOCUS or SDL_APPACTIVE */
            SET_INT(active, state);

            lua_rawset(L, -3);
            break;

        case SDL_JOYAXISMOTION:
            lua_pushliteral(L, "jaxis");
            lua_createtable(L, 0, 4);

            /* SDL_JOYAXISMOTION */
            SET_INT(jaxis, type);

            SET_INT(jaxis, which);
            SET_INT(jaxis, axis);
            SET_INT(jaxis, value);

            lua_rawset(L, -3);
            break;

        case SDL_JOYBALLMOTION:
            lua_pushliteral(L, "jball");
            lua_createtable(L, 0, 5);

            /* SDL_JOYBALLMOTION */
            SET_INT(jball, type);

            SET_INT(jball, which);
            SET_INT(jball, ball);
            SET_INT(jball, xrel);
            SET_INT(jball, yrel);

            lua_rawset(L, -3);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            lua_pushliteral(L, "jbutton");
            lua_createtable(L, 0, 4);

            /* SDL_JOYBUTTONDOWN/UP */
            SET_INT(jbutton, type);

            SET_INT(jbutton, which);
            SET_INT(jbutton, button);

            /* SDL_PRESSED/RELEASED */
            SET_INT(jbutton, state);

            lua_rawset(L, -3);
            break;

        case SDL_JOYHATMOTION:
            lua_pushliteral(L, "jhat");
            lua_createtable(L, 0, 4);

            /* SDL_JOYHATMOTION */
            SET_INT(jhat, type);

            SET_INT(jhat, which);
            SET_INT(jhat, hat);

            lua_pushliteral(L, "value");
            HatPositionToTable(L, event.jhat.value);
            lua_rawset(L, -3);

            lua_rawset(L, -3);
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            lua_pushliteral(L, "key");
            lua_createtable(L, 0, 3);

            /* SDL_KEYDOWN/UP */
            SET_INT(key, type);

            /* SDL_PRESSED/RELEASED */
            SET_INT(key, state);

            lua_pushliteral(L, "keysym");
            KeysymToTable(L, event.key.keysym);
            lua_rawset(L, -3);

            lua_rawset(L, -3);
            break;

        case SDL_MOUSEMOTION:
            lua_pushliteral(L, "motion");
            lua_createtable(L, 0, 6);

            /* SDL_MOUSEMOTION */
            SET_INT(motion, type);

            lua_pushliteral(L, "state");
            ButtonStateToTable(L, event.motion.state);
            lua_rawset(L, -3);

            SET_INT(motion, x);
            SET_INT(motion, y);
            SET_INT(motion, xrel);
            SET_INT(motion, yrel);

            lua_rawset(L, -3);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            lua_pushliteral(L, "button");
            lua_createtable(L, 0, 5);

            /* SDL_MOUSEBUTTONDOWN/UP */
            SET_INT(button, type);

            /* SDL_BUTTON_LEFT/MIDDLE/RIGHT */
            SET_INT(button, button);

            /* SDL_PRESSED or SDL_RELEASED */
            SET_INT(button, state);

            SET_INT(button, x);
            SET_INT(button, y);

            lua_rawset(L, -3);
            break;

        case SDL_QUIT:
            lua_pushliteral(L, "quit");
            lua_createtable(L, 0, 1);

            /* SDL_QUIT */
            SET_INT(quit, type);

            lua_rawset(L, -3);
            break;

        /* SDL_USEREVENT through to (SDL_NUMEVENTS - 1) are handled in the default case. */

        case SDL_VIDEOEXPOSE:
            lua_pushliteral(L, "expose");
            lua_createtable(L, 0, 1);

            /* SDL_VIDEOEXPOSE */
            SET_INT(expose, type);

            lua_rawset(L, -3);
            break;

        case SDL_VIDEORESIZE:
            lua_pushliteral(L, "resize");
            lua_createtable(L, 0, 3);

            /* SDL_VIDEORESIZE */
            SET_INT(resize, type);

            SET_INT(resize, w);
            SET_INT(resize, h);

            lua_rawset(L, -3);
            break;

        default:
            if (event.type >= SDL_USEREVENT && event.type < SDL_NUMEVENTS) {
                lua_pushliteral(L, "user");
                lua_createtable(L, 0, 4);

                /* SDL_USEREVENT through to (SDL_NUMEVENTS - 1) */
                SET_INT(user, type);

                SET_INT(user, code);

                /* Back to Lua-land, so the ref is no longer needed. */
                lua_pushliteral(L, "data1");
                lua_rawgeti(L, LUA_REGISTRYINDEX, (int)event.user.data1);
                lua_rawset(L, -3);
                luaL_unref(L, LUA_REGISTRYINDEX, (int)event.user.data1);

                /* ^ ditto */
                lua_pushliteral(L, "data2");
                lua_rawgeti(L, LUA_REGISTRYINDEX, (int)event.user.data2);
                lua_rawset(L, -3);
                luaL_unref(L, LUA_REGISTRYINDEX, (int)event.user.data2);

                lua_rawset(L, -3);
            } else {
                luaL_error(L, "could not convert unrecognized event (%d)", event.type);
            }
    }
    #undef SET_INT
}



/**
 * Exported SDL Event Functions
 */

/* number key -> string key_name */
static int GetKeyName(lua_State *L) {
    SDLKey key = luaL_checkint(L, 1);
    const char *key_name = SDL_GetKeyName(key);
    lua_pushstring(L, key_name);
    return 1;
}


/* -> array buttons_state{0..8}, number x, number y */
static int GetMouseState(lua_State *L) {
    int x;
    int y;
    Uint8 buttons = SDL_GetMouseState(&x, &y);
    ButtonStateToTable(L, buttons);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 3;
}


/* -> (nil | table event) */
static int PollEvent(lua_State *L) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        EventToTable(L, event);
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
    luaL_checktype(L, 1, LUA_TTABLE);

    /* Get the event type. */
    lua_pushliteral(L, "type");
    lua_gettable(L, 1);
    int type = lua_tointeger(L, -1);
    SDL_Event event;
    event.type = type;

    #define PUSH_EVENT(st, n) \
        do { \
            event.n.type = type; \
            lua_pushliteral(st, #n); \
            lua_gettable(st, 1); \
        } while (0)

    #define GET_INT(t, n) \
        do { \
            lua_pushliteral(L, #n); \
            lua_gettable(L, -2); \
            event.t.n = lua_tointeger(L, -1); \
            lua_pop(L, 1); \
        } while (0)

    #define POP_EVENT(st) \
        do { \
            lua_pop(st, 1); \
        } while (0)

    switch (event.type) {
        case SDL_ACTIVEEVENT:
            PUSH_EVENT(L, active);
            GET_INT(active, gain);
            GET_INT(active, state);
            POP_EVENT(L);
            break;

        case SDL_JOYAXISMOTION:
            PUSH_EVENT(L, jaxis);
            GET_INT(jaxis, which);
            GET_INT(jaxis, axis);
            GET_INT(jaxis, value);
            POP_EVENT(L);
            break;

        case SDL_JOYBALLMOTION:
            PUSH_EVENT(L, jball);
            GET_INT(jball, which);
            GET_INT(jball, ball);
            GET_INT(jball, xrel);
            GET_INT(jball, yrel);
            POP_EVENT(L);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            PUSH_EVENT(L, jbutton);
            GET_INT(jbutton, which);
            GET_INT(jbutton, button);
            GET_INT(jbutton, state);
            POP_EVENT(L);
            break;

        case SDL_JOYHATMOTION:
            PUSH_EVENT(L, jhat);
            GET_INT(jhat, which);
            GET_INT(jhat, hat);

            lua_pushliteral(L, "value");
            lua_gettable(L, -2);
            event.jhat.value = TableToHatPosition(L, -1);
            lua_pop(L, 1);

            POP_EVENT(L);
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            PUSH_EVENT(L, key);
            GET_INT(key, state);

            lua_pushliteral(L, "keysym");
            lua_gettable(L, -2);
            TableToKeysym(L, -1, &event);
            lua_pop(L, 1);

            POP_EVENT(L);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            PUSH_EVENT(L, button);
            GET_INT(button, button);
            GET_INT(button, state);
            GET_INT(button, x);
            GET_INT(button, y);
            POP_EVENT(L);
            break;

        case SDL_MOUSEMOTION:
            PUSH_EVENT(L, motion);

            lua_pushliteral(L, "state");
            lua_gettable(L, -2);
            event.motion.state = TableToButtonState(L, -1);
            lua_pop(L, 1);

            GET_INT(motion, x);
            GET_INT(motion, y);
            GET_INT(motion, xrel);
            GET_INT(motion, yrel);
            POP_EVENT(L);
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
            PUSH_EVENT(L, resize);
            GET_INT(resize, w);
            GET_INT(resize, h);
            POP_EVENT(L);
            break;

        default:
            if (type >= SDL_USEREVENT && type < SDL_NUMEVENTS) {
                PUSH_EVENT(L, user);
                GET_INT(user, code);

                /* Ensure Lua hangs onto whatever data is here, even as it travels through SDL's event queue. */
                lua_pushliteral(L, "data1");
                lua_gettable(L, -2);
                event.user.data1 = (void *)luaL_ref(L, LUA_REGISTRYINDEX);

                /* ^ ditto */
                lua_pushliteral(L, "data2");
                lua_gettable(L, -2);
                event.user.data2 = (void *)luaL_ref(L, LUA_REGISTRYINDEX);

                POP_EVENT(L);
            } else {
                luaL_error(L, "could not push unrecognized event (%d)", type);
            }
            break;
    }

    #undef PUSH_EVENT
    #undef GET_INT
    #undef POP_EVENT

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
    EventToTable(L, event);
    return 1;
}



/**
 * Public API
 */

#define DEF_E_CONST(name) {#name, SDL_##name}
static const luasdl_NameConst8 m_sdl_event_constants[] = {
    /* SDL event constants */
    DEF_E_CONST(ACTIVEEVENT),
    DEF_E_CONST(JOYAXISMOTION),
    DEF_E_CONST(JOYBALLMOTION),
    DEF_E_CONST(JOYBUTTONDOWN),
    DEF_E_CONST(JOYBUTTONUP),
    DEF_E_CONST(JOYHATMOTION),
    DEF_E_CONST(KEYDOWN),
    DEF_E_CONST(KEYUP),
    DEF_E_CONST(MOUSEBUTTONDOWN),
    DEF_E_CONST(MOUSEBUTTONUP),
    DEF_E_CONST(MOUSEMOTION),
    DEF_E_CONST(QUIT),
    /* SDL_SYSWMEVENT omitted due to massive cross-platform effort needed. */
    DEF_E_CONST(USEREVENT),
    DEF_E_CONST(VIDEOEXPOSE),
    DEF_E_CONST(VIDEORESIZE),
    DEF_E_CONST(NUMEVENTS),

    /* SDL_ACTIVEEVENT state constants */
    DEF_E_CONST(APPMOUSEFOCUS),
    DEF_E_CONST(APPINPUTFOCUS),
    DEF_E_CONST(APPACTIVE),

    /* mouse button constants */
    DEF_E_CONST(BUTTON_LEFT),
    DEF_E_CONST(BUTTON_MIDDLE),
    DEF_E_CONST(BUTTON_RIGHT),

    /* press state constants */
    DEF_E_CONST(PRESSED),
    DEF_E_CONST(RELEASED),

    {NULL, 0}
};
#undef DEF_E_CONST

/* Load SDL event constants into SDL module table at index. */
static void LoadSdlEventConstants(lua_State *L, int index) {
    const luasdl_NameConst8 *p;
    for (p = m_sdl_event_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->uint);
        lua_settable(L, index < 0 ? index - 2 : index);
    }
}


#define DEF_KEY(name) {#name, SDLK_##name}
static const luasdl_NameSdlKey m_sdl_key_constants[] = {
    DEF_KEY(BACKSPACE),
    DEF_KEY(TAB),
    DEF_KEY(CLEAR),
    DEF_KEY(RETURN),
    DEF_KEY(PAUSE),
    DEF_KEY(ESCAPE),
    DEF_KEY(SPACE),
    DEF_KEY(EXCLAIM),
    DEF_KEY(QUOTEDBL),
    DEF_KEY(HASH),
    DEF_KEY(DOLLAR),
    DEF_KEY(AMPERSAND),
    DEF_KEY(QUOTE),
    DEF_KEY(LEFTPAREN),
    DEF_KEY(RIGHTPAREN),
    DEF_KEY(ASTERISK),
    DEF_KEY(PLUS),
    DEF_KEY(COMMA),
    DEF_KEY(MINUS),
    DEF_KEY(PERIOD),
    DEF_KEY(SLASH),
    DEF_KEY(0),
    DEF_KEY(1),
    DEF_KEY(2),
    DEF_KEY(3),
    DEF_KEY(4),
    DEF_KEY(5),
    DEF_KEY(6),
    DEF_KEY(7),
    DEF_KEY(8),
    DEF_KEY(9),
    DEF_KEY(COLON),
    DEF_KEY(SEMICOLON),
    DEF_KEY(LESS),
    DEF_KEY(EQUALS),
    DEF_KEY(GREATER),
    DEF_KEY(QUESTION),
    DEF_KEY(AT),
    DEF_KEY(LEFTBRACKET),
    DEF_KEY(BACKSLASH),
    DEF_KEY(RIGHTBRACKET),
    DEF_KEY(CARET),
    DEF_KEY(UNDERSCORE),
    DEF_KEY(BACKQUOTE),
    DEF_KEY(a),
    DEF_KEY(b),
    DEF_KEY(c),
    DEF_KEY(d),
    DEF_KEY(e),
    DEF_KEY(f),
    DEF_KEY(g),
    DEF_KEY(h),
    DEF_KEY(i),
    DEF_KEY(j),
    DEF_KEY(k),
    DEF_KEY(l),
    DEF_KEY(m),
    DEF_KEY(n),
    DEF_KEY(o),
    DEF_KEY(p),
    DEF_KEY(q),
    DEF_KEY(r),
    DEF_KEY(s),
    DEF_KEY(t),
    DEF_KEY(u),
    DEF_KEY(v),
    DEF_KEY(w),
    DEF_KEY(x),
    DEF_KEY(y),
    DEF_KEY(z),
    DEF_KEY(DELETE),
    DEF_KEY(KP0),
    DEF_KEY(KP1),
    DEF_KEY(KP2),
    DEF_KEY(KP3),
    DEF_KEY(KP4),
    DEF_KEY(KP5),
    DEF_KEY(KP6),
    DEF_KEY(KP7),
    DEF_KEY(KP8),
    DEF_KEY(KP9),
    DEF_KEY(KP_PERIOD),
    DEF_KEY(KP_DIVIDE),
    DEF_KEY(KP_MULTIPLY),
    DEF_KEY(KP_MINUS),
    DEF_KEY(KP_PLUS),
    DEF_KEY(KP_ENTER),
    DEF_KEY(KP_EQUALS),
    DEF_KEY(UP),
    DEF_KEY(DOWN),
    DEF_KEY(RIGHT),
    DEF_KEY(LEFT),
    DEF_KEY(INSERT),
    DEF_KEY(HOME),
    DEF_KEY(END),
    DEF_KEY(PAGEUP),
    DEF_KEY(PAGEDOWN),
    DEF_KEY(F1),
    DEF_KEY(F2),
    DEF_KEY(F3),
    DEF_KEY(F4),
    DEF_KEY(F5),
    DEF_KEY(F6),
    DEF_KEY(F7),
    DEF_KEY(F8),
    DEF_KEY(F9),
    DEF_KEY(F10),
    DEF_KEY(F11),
    DEF_KEY(F12),
    DEF_KEY(F13),
    DEF_KEY(F14),
    DEF_KEY(F15),
    DEF_KEY(NUMLOCK),
    DEF_KEY(CAPSLOCK),
    DEF_KEY(SCROLLOCK),
    DEF_KEY(RSHIFT),
    DEF_KEY(LSHIFT),
    DEF_KEY(RCTRL),
    DEF_KEY(LCTRL),
    DEF_KEY(RALT),
    DEF_KEY(LALT),
    DEF_KEY(RMETA),
    DEF_KEY(LMETA),
    DEF_KEY(LSUPER),
    DEF_KEY(RSUPER),
    DEF_KEY(MODE),
    DEF_KEY(HELP),
    DEF_KEY(PRINT),
    DEF_KEY(SYSREQ),
    DEF_KEY(BREAK),
    DEF_KEY(MENU),
    DEF_KEY(POWER),
    DEF_KEY(EURO),
    {NULL, 0}
};
#undef DEF_KEY

/* Load SDL key constants into SDL module table at index. */
static void LoadSdlKeyConstants(lua_State *L, int index) {
    const luasdl_NameSdlKey *p;
    lua_createtable(L, 0, sizeof(m_sdl_key_constants) / sizeof(luasdl_NameSdlKey));
    for (p = m_sdl_key_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->key);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
    lua_setglobal(L, "SDLK");
}


#define DEF_KMOD(name) {#name, KMOD_##name}
static const luasdl_NameSdlMod m_sdl_kmod_constants[] = {
    DEF_KMOD(LSHIFT),
    DEF_KMOD(RSHIFT),
    DEF_KMOD(LCTRL),
    DEF_KMOD(RCTRL),
    DEF_KMOD(LALT),
    DEF_KMOD(RALT),
    DEF_KMOD(LMETA),
    DEF_KMOD(RMETA),
    DEF_KMOD(NUM),
    DEF_KMOD(CAPS),
    DEF_KMOD(MODE),
    {NULL, 0}
};
#undef DEF_KMOD

/* Load SDL key modifier constants into SDL module table at index. */
static void LoadSdlKmodConstants(lua_State *L, int index) {
    const luasdl_NameSdlMod *p;
    lua_createtable(L, 0, sizeof(m_sdl_kmod_constants) / sizeof(luasdl_NameSdlMod));
    for (p = m_sdl_kmod_constants; p->name != NULL; p++) {
        lua_pushstring(L, p->name);
        lua_pushinteger(L, p->mod);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
    lua_setglobal(L, "KMOD");
}


static const luaL_reg m_sdl_event_functions [] = {
    {"GetKeyName", GetKeyName},
    {"GetMouseState", GetMouseState},
    {"PollEvent", PollEvent},
    {"PumpEvents", PumpEvents},
    {"PushEvent", PushEvent},
    {"WaitEvent", WaitEvent},
    {NULL, NULL}
};

/* Load SDL event function into SDL module table at index. */
static void LoadSdlEventFunctions(lua_State *L, int index) {
    const luaL_Reg *reg;
    for (reg = m_sdl_event_functions; reg->name != NULL; reg++) {
        lua_pushstring(L, reg->name);
        lua_pushcfunction(L, reg->func);
        lua_rawset(L, index < 0 ? index - 2 : index);
    }
}


/* Load event API into SDL module table at index. */
void LoadSdlEvent(lua_State *L, int index) {
    LoadSdlEventFunctions(L, index);
    LoadSdlEventConstants(L, index);
    LoadSdlKeyConstants(L, index);
    LoadSdlKmodConstants(L, index);
}
