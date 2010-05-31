CFLAGS:=-std=c99 -W -Wall -Wextra

### Tasks ###

.PHONY: all clean

all: quipkit lib/png/png.so lib/sdl/sdl.so

clean:
	-rm -f quipkit *.o
	-rm -f lib/png/png.so lib/png/*.o
	-rm -f lib/sdl/sdl.so lib/sdl/*.o

### Quipkit ###

QK_CFLAGS:=${CFLAGS} `pkg-config --cflags lua5.1`
QK_LIBS:=`pkg-config --libs lua5.1`

quipkit: quipkit.o
	gcc -o $@ ${QK_LIBS} $^

quipkit.o: quipkit.c
	gcc -o $@ ${QK_CFLAGS} -c $^

### libpng bindings ###

QK_PNG_CFLAGS:=${QK_CFLAGS} `pkg-config --cflags libpng`
QK_PNG_LIBS:=${QK_LIBS} `pkg-config --libs libpng`

lib/png/png.so: lib/png/png.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${QK_PNG_LIBS} $+

lib/png/png.o: lib/png/png.c
	gcc -fPIC -o $@ ${QK_PNG_CFLAGS} -c $<

### SDL bindings ###

QK_SDL_CFLAGS:=${QK_CFLAGS} `sdl-config --cflags`
QK_SDL_LIBS:=${QK_LIBS} `sdl-config --libs`

lib/sdl/sdl.so: lib/sdl/sdl.o lib/sdl/event.o lib/sdl/gl.o lib/sdl/video.o lib/sdl/wm.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${QK_SDL_LIBS} $+

lib/sdl/sdl.o: lib/sdl/sdl.c lib/sdl/name_Uint_pair.h lib/sdl/event.h lib/sdl/gl.h lib/sdl/video.h lib/sdl/wm.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/event.o: lib/sdl/event.c lib/sdl/event.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/gl.o: lib/sdl/gl.c lib/sdl/gl.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/video.o: lib/sdl/video.c lib/sdl/video.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
lib/sdl/wm.o: lib/sdl/wm.c lib/sdl/wm.h
	gcc -fPIC -o $@ ${QK_SDL_CFLAGS} -c $<
