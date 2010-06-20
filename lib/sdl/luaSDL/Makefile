# Makefile for luaSDL -Thatcher Ulrich

# Public Domain


include config.mak

LUA := ../../lua-4.0-loadmodule
LUA_INCLUDE := $(LUA)/include
LUA_LIBS := $(LUA)/lib/$(LIB_PRE)lua.$(LIB_EXT)

CFLAGS := $(CFLAGS) -I$(LUA_INCLUDE) $(CC_DEBUG_FLAGS)
LDFLAGS := $(SDL_LDFLAGS) $(LDFLAGS)

TOLUA_LIBS := -ltolua

LUA_SDL_LIBS := $(TOLUA_LIBS) $(LUA_LIBS) $(SDL_LDFLAGS)

all: $(LIB_PRE)luaSDL.$(DLL_EXT)

SDL_tolua.c: SDL.pkg
	tolua -n SDL -o $@ $^

SDL_tolua.$(OBJ_EXT): SDL_tolua.c
	$(CC) -c $^ -I../../../tolua/include -I$(LUA_INCLUDE)

$(LIB_PRE)luaSDL.$(DLL_EXT): SDL_tolua.$(OBJ_EXT)
	$(MAKE_DLL) -o $@ $^ $(LUA_SDL_LIBS)


clean:
	-rm *~
	-rm SDL_tolua.$(OBJ_EXT)
	-rm SDL_tolua.c
	-rm $(LIB_PRE)luaSDL.$(DLL_EXT)
