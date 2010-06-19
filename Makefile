CFLAGS:=-std=c99 -W -Wall -Wextra



LIBPNG_CFLAGS:=`pkg-config --cflags libpng`
LIBPNG_LIBS:=`pkg-config --libs libpng`

LUA_CFLAGS:=`pkg-config --cflags lua5.1`
LUA_LIBS:=`pkg-config --libs lua5.1`

SDL_CFLAGS:=`sdl-config --cflags`
SDL_LIBS:=`sdl-config --libs`

TCLTK_CFLAGS:=-I/usr/include/tcl8.5
TCLTK_LIBS:=-ltcl8.5



LUAPNG_DIR:=lib/png/
LUASDL_DIR:=lib/sdl/
LTCLTK_DIR:=dev/lib/tcltk/ltcltk-0.9-1/



### Tasks ###

.PHONY: all clean

all: game ${LUAPNG_DIR}libluapng.so ${LUASDL_DIR}libluasdl.so edit ${LTCLTK_DIR}ltcl.so

clean:
	-rm -f game edit *.o
	-rm -f ${LUAPNG_DIR}libluapng.so ${LUAPNG_DIR}*.o
	-rm -f ${LUASDL_DIR}libluasdl.so ${LUASDL_DIR}*.o
	-rm -f ${LTCLTK_DIR}ltcl.so ${LTCLTK_DIR}*.o



### Quipkit Engine ###

QKENG_CFLAGS:=${CFLAGS} ${LUA_CFLAGS}
QKENG_LIBS:=${LUA_LIBS}

game: game.o
	gcc -o $@ ${QKENG_LIBS} $^

game.o: game.c
	gcc -o $@ ${QKENG_CFLAGS} -c $^

### libpng bindings ###

QK_PNG_CFLAGS:=${QKENG_CFLAGS} ${LIBPNG_CFLAGS}
QK_PNG_LIBS:=${QKENG_LIBS} ${LIBPNG_LIBS}

${LUAPNG_DIR}libluapng.so: ${LUAPNG_DIR}png.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${QK_PNG_LIBS} $+

${LUAPNG_DIR}png.o: ${LUAPNG_DIR}png.c ${LUAPNG_DIR}png.h
	gcc -fPIC -o $@ ${QK_PNG_CFLAGS} -c $<

### SDL bindings ###

QK_SDL_CFLAGS:=${QKENG_CFLAGS} ${SDL_CFLAGS}
QK_SDL_LIBS:=${QKENG_LIBS} ${SDL_LIBS}

${LUASDL_DIR}libluasdl.so: ${LUASDL_DIR}sdl.o ${LUASDL_DIR}event.o ${LUASDL_DIR}gl.o ${LUASDL_DIR}surface.o ${LUASDL_DIR}video.o ${LUASDL_DIR}wm.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${QK_SDL_LIBS} $+

${LUASDL_DIR}sdl.o: ${LUASDL_DIR}sdl.c ${LUASDL_DIR}sdl.h ${LUASDL_DIR}event.h ${LUASDL_DIR}gl.h ${LUASDL_DIR}types.h ${LUASDL_DIR}video.h ${LUASDL_DIR}wm.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
${LUASDL_DIR}event.o: ${LUASDL_DIR}event.c ${LUASDL_DIR}event.h ${LUASDL_DIR}types.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
${LUASDL_DIR}gl.o: ${LUASDL_DIR}gl.c ${LUASDL_DIR}gl.h ${LUASDL_DIR}types.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
${LUASDL_DIR}surface.o: ${LUASDL_DIR}surface.c ${LUASDL_DIR}surface.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
${LUASDL_DIR}video.o: ${LUASDL_DIR}video.c ${LUASDL_DIR}video.h ${LUASDL_DIR}surface.h ${LUASDL_DIR}types.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
${LUASDL_DIR}wm.o: ${LUASDL_DIR}wm.c ${LUASDL_DIR}wm.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<



### Quipkit Editor ###

QKED_CFLAGS:=${CFLAGS} ${LUA_CFLAGS}
QKED_LIBS:=${LUA_LIBS}

edit: edit.o
	gcc -o $@ ${QKED_LIBS} $^

edit.o: edit.c
	gcc -o $@ ${QKED_CFLAGS} -c $^

### Tcl bindings (and thus Tk too) ###

LTCLTK_CFLAGS:=${QKED_CFLAGS} ${TCLTK_CFLAGS}
LTCLTK_LIBS:=${QKED_LIBS} ${TCLTK_LIBS}

${LTCLTK_DIR}ltcl.so: ${LTCLTK_DIR}ltcl.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${LTCLTK_LIBS} $+

${LTCLTK_DIR}ltcl.o: ${LTCLTK_DIR}ltcl.c
	gcc -fPIC -o $@ ${LTCLTK_CFLAGS} -c $<
