CFLAGS:=-std=c99 -W -Wall -Wextra

### Tasks ###

.PHONY: all clean

all: game lib/png/libluapng.so lib/sdl/libluasdl.so

clean:
	-rm -f game *.o
	-rm -f lib/png/libluapng.so lib/png/*.o
	-rm -f lib/sdl/libluasdl.so lib/sdl/*.o

### Quipkit Engine ###

QKENG_CFLAGS:=${CFLAGS} `pkg-config --cflags lua5.1`
QKENG_LIBS:=`pkg-config --libs lua5.1`

game: game.o
	gcc -o $@ ${QKENG_LIBS} $^

game.o: game.c
	gcc -o $@ ${QKENG_CFLAGS} -c $^

### libpng bindings ###

QK_PNG_CFLAGS:=${QKENG_CFLAGS} `pkg-config --cflags libpng`
QK_PNG_LIBS:=${QKENG_LIBS} `pkg-config --libs libpng`

lib/png/libluapng.so: lib/png/png.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${QK_PNG_LIBS} $+

lib/png/png.o: lib/png/png.c lib/png/png.h
	gcc -fPIC -o $@ ${QK_PNG_CFLAGS} -c $<

### SDL bindings ###

QK_SDL_CFLAGS:=${QKENG_CFLAGS} `sdl-config --cflags`
QK_SDL_LIBS:=${QKENG_LIBS} `sdl-config --libs`

lib/sdl/libluasdl.so: lib/sdl/sdl.o lib/sdl/event.o lib/sdl/gl.o lib/sdl/surface.o lib/sdl/video.o lib/sdl/wm.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${QK_SDL_LIBS} $+

lib/sdl/sdl.o: lib/sdl/sdl.c lib/sdl/sdl.h lib/sdl/name_Uint_pair.h lib/sdl/event.h lib/sdl/gl.h lib/sdl/video.h lib/sdl/wm.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/event.o: lib/sdl/event.c lib/sdl/event.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/gl.o: lib/sdl/gl.c lib/sdl/gl.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/surface.o: lib/sdl/surface.c lib/sdl/surface.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/video.o: lib/sdl/video.c lib/sdl/video.h lib/sdl/name_Uint_pair.h lib/sdl/surface.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/wm.o: lib/sdl/wm.c lib/sdl/wm.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
