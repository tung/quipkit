CFLAGS:=-W -Wall -Wextra

### Tasks ###

.PHONY: all clean

all: quipkit lib/glpng/glpng.so lib/sdl/sdl.so

clean:
	rm -f quipkit *.o
	rm -f lib/glpng/glpng.so lib/glpng/*.o
	rm -f lib/sdl/sdl.so lib/sdl/*.o

### Quipkit ###

quipkit: quipkit.o
	gcc -o quipkit \
		${CFLAGS} \
		`pkg-config --libs lua` \
		quipkit.o

quipkit.o: quipkit.c
	gcc -o quipkit.o \
		${CFLAGS} \
		`pkg-config --cflags lua` \
		-c quipkit.c

### glpng bindings ###

lib/glpng/glpng.so: lib/glpng/glpng.o
	ld -shared -soname glpng.so -o lib/glpng/glpng.so \
		-lglpng `pkg-config --libs gl` `pkg-config --libs lua` \
		lib/glpng/glpng.o

lib/glpng/glpng.o: lib/glpng/glpng.c
	gcc -fPIC -o lib/glpng/glpng.o \
		${CFLAGS} \
		`pkg-config --cflags gl` \
		-c lib/glpng/glpng.c

### SDL bindings ###

lib/sdl/sdl.so: lib/sdl/sdl.o lib/sdl/event.o lib/sdl/gl.o lib/sdl/video.o lib/sdl/wm.o
	ld -shared -soname sdl.so -o lib/sdl/sdl.so \
		`sdl-config --libs` `pkg-config --libs lua` \
		lib/sdl/sdl.o lib/sdl/event.o lib/sdl/gl.o lib/sdl/video.o lib/sdl/wm.o

lib/sdl/sdl.o: lib/sdl/sdl.c lib/sdl/name_Uint_pair.h lib/sdl/event.h lib/sdl/gl.h lib/sdl/video.h lib/sdl/wm.h
	gcc -fPIC -o lib/sdl/sdl.o \
		${CFLAGS} \
		`sdl-config --cflags` `pkg-config --cflags lua` \
		-c lib/sdl/sdl.c

lib/sdl/event.o: lib/sdl/event.c lib/sdl/event.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o lib/sdl/event.o \
		`sdl-config --cflags` `pkg-config --cflags lua` \
		${CFLAGS} \
		-c lib/sdl/event.c

lib/sdl/gl.o: lib/sdl/gl.c lib/sdl/gl.h
	gcc -fPIC -o lib/sdl/gl.o \
		`sdl-config --cflags` `pkg-config --cflags lua` \
		${CFLAGS} \
		-c lib/sdl/gl.c

lib/sdl/video.o: lib/sdl/video.c lib/sdl/video.h lib/sdl/name_Uint_pair.h
	gcc -fPIC -o lib/sdl/video.o \
		${CFLAGS} \
		`sdl-config --cflags` `pkg-config --cflags lua` \
		-c lib/sdl/video.c

lib/sdl/wm.o: lib/sdl/wm.c lib/sdl/wm.h
	gcc -fPIC -o lib/sdl/wm.o \
		${CFLAGS} \
		`sdl-config --cflags` `pkg-config --cflags lua` \
		-c lib/sdl/wm.c
