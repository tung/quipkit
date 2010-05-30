CFLAGS:=-std=c99 -W -Wall -Wextra

### Tasks ###

.PHONY: all clean

all: quipkit lib/sdl/sdl.so

clean:
	-rm -f quipkit *.o
	-rm -f lib/sdl/sdl.so lib/sdl/*.o

### Quipkit ###

quipkit: quipkit.o
	gcc -o quipkit \
		${CFLAGS} \
		`pkg-config --libs lua5.1` \
		quipkit.o

quipkit.o: quipkit.c
	gcc -o quipkit.o \
		${CFLAGS} \
		`pkg-config --cflags lua5.1` \
		-c quipkit.c

### SDL bindings ###

lib/sdl/sdl.so: lib/sdl/sdl.o lib/sdl/event.o lib/sdl/gl.o lib/sdl/video.o lib/sdl/wm.o
	ld -shared -soname sdl.so -o lib/sdl/sdl.so \
		`sdl-config --libs` `pkg-config --libs lua5.1` \
		lib/sdl/sdl.o lib/sdl/event.o lib/sdl/gl.o lib/sdl/video.o lib/sdl/wm.o

lib/sdl/sdl.o: lib/sdl/sdl.c lib/sdl/name_Uint_pair.h lib/sdl/event.h lib/sdl/gl.h lib/sdl/video.h lib/sdl/wm.h
	gcc -fPIC -o lib/sdl/sdl.o \
		${CFLAGS} \
		`sdl-config --cflags` `pkg-config --cflags lua5.1` \
		-c lib/sdl/sdl.c

lib/sdl/event.o: lib/sdl/event.c lib/sdl/event.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o lib/sdl/event.o \
		`sdl-config --cflags` `pkg-config --cflags lua5.1` \
		${CFLAGS} \
		-c lib/sdl/event.c

lib/sdl/gl.o: lib/sdl/gl.c lib/sdl/gl.h
	gcc -fPIC -o lib/sdl/gl.o \
		`sdl-config --cflags` `pkg-config --cflags lua5.1` \
		${CFLAGS} \
		-c lib/sdl/gl.c

lib/sdl/video.o: lib/sdl/video.c lib/sdl/video.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o lib/sdl/video.o \
		${CFLAGS} \
		`sdl-config --cflags` `pkg-config --cflags lua5.1` \
		-c lib/sdl/video.c

lib/sdl/wm.o: lib/sdl/wm.c lib/sdl/wm.h
	gcc -fPIC -o lib/sdl/wm.o \
		${CFLAGS} \
		`sdl-config --cflags` `pkg-config --cflags lua5.1` \
		-c lib/sdl/wm.c
