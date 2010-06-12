#ifndef QUIPKIT_NAME_UINT_PAIR_H_INCLUDED
#define QUIPKIT_NAME_UINT_PAIR_H_INCLUDED

typedef struct {
    const char *name;
    Uint8 uint;
} luasdl_NameConst8;

typedef struct {
    const char *name;
    Uint32 uint;
} luasdl_NameConst32;

typedef struct {
    const char *name;
    SDLKey key;
} luasdl_NameSdlKey;

typedef struct {
    const char *name;
    SDLMod mod;
} luasdl_NameSdlMod;

typedef struct {
    const char *name;
    SDL_GLattr attr;
} luasdl_NameSdlGlAttr;

#endif
