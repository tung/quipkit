CFLAGS:=-std=c99 -W -Wall -Wextra



ALSA_CFLAGS:=
ALSA_LIBS:=-lasound

GL_CFLAGS:=`pkg-config --cflags gl`
GL_LIBS:=`pkg-config --libs gl`
GLU_CFLAGS:=
GLU_LIBS:=-lGLU

LUA_CFLAGS:=`pkg-config --cflags lua5.1`
LUA_LIBS:=`pkg-config --libs lua5.1`

SDL_CFLAGS:=`sdl-config --cflags`
SDL_LIBS:=`sdl-config --libs`


LUAGL_DIR:=lib/gl/luagl/
LUASDL_DIR:=lib/sdl/LuaSDL_new/
LUASDL_DIR_BACK:=../../../
PROTEAAUDIO_DIR:=lib/proteaAudio/
SDLGL_DIR:=lib/sdlgl/
TOLUAPP_DIR:=contrib/tolua++/



### Tasks ###

.PHONY: all
all: game all_luagl ${LUASDL_DIR}libluasdl.so all_toluapp ${PROTEAAUDIO_DIR}libproaudio.so ${SDLGL_DIR}libsdlgl.so

.PHONY: clean
clean: clean_game clean_luagl clean_luasdl clean_toluapp clean_proteaaudio clean_sdlgl



### Quipkit Engine ###

QKENG_CFLAGS:=${CFLAGS} ${LUA_CFLAGS}
QKENG_LIBS:=${LUA_LIBS}

game: game.o
	gcc -o $@ ${QKENG_LIBS} $^

game.o: game.c
	gcc -o $@ ${QKENG_CFLAGS} -c $^

.PHONY: clean_game
clean_game:
	-rm -f game
	-rm -f game.o


### LuaGL ###

LUAGL_CFLAGS:=-Wall -O2
# No LUAGL_LIBS because none of these modules share common lib flags.

.PHONY: all_luagl
all_luagl: ${LUAGL_DIR}libluagl.so ${LUAGL_DIR}libluaglu.so

${LUAGL_DIR}libluagl.so: ${LUAGL_DIR}luagl.o ${LUAGL_DIR}luagl_const.o ${LUAGL_DIR}luagl_util.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${GL_LIBS} $^

${LUAGL_DIR}luagl.o: ${LUAGL_DIR}luagl.c ${LUAGL_DIR}luagl.h ${LUAGL_DIR}luagl_const.h ${LUAGL_DIR}luagl_util.h
	gcc -fPIC -o $@ ${LUAGL_CFLAGS} ${LUA_CFLAGS} ${GL_CFLAGS} -c $<

${LUAGL_DIR}luagl_const.o: ${LUAGL_DIR}luagl_const.c ${LUAGL_DIR}luagl_const.h ${LUAGL_DIR}luagl_util.h
	gcc -fPIC -o $@ ${LUAGL_CFLAGS} ${LUA_CFLAGS} ${GL_CFLAGS} -c $<

${LUAGL_DIR}luagl_util.o: ${LUAGL_DIR}luagl_util.c ${LUAGL_DIR}luagl_util.h
	gcc -fPIC -o $@ ${LUAGL_CFLAGS} ${LUA_CFLAGS} -c $<

${LUAGL_DIR}libluaglu.so: ${LUAGL_DIR}luaglu.o ${LUAGL_DIR}luagl_const.o ${LUAGL_DIR}luagl_util.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${GL_LIBS} ${GLU_LIBS} $^

${LUAGL_DIR}luaglu.o: ${LUAGL_DIR}luaglu.c ${LUAGL_DIR}luaglu.h ${LUAGL_DIR}luagl_const.h ${LUAGL_DIR}luagl_util.h
	gcc -fPIC -o $@ ${LUAGL_CFLAGS} ${LUA_CFLAGS} ${GL_CFLAGS} ${GLU_CFLAGS} -c $<

.PHONY: clean_luagl
clean_luagl:
	-rm -f ${LUAGL_DIR}*.so
	-rm -f ${LUAGL_DIR}*.o


### New LuaSDL bindings from Kein-Hong Man (needs tolua++) ###

# Kein-Hong Man's LuaSDL breaks on -std=c99 due to some inline function issues.
#LUASDL_CFLAGS:=${QKENG_CFLAGS} ${SDL_CFLAGS}
LUASDL_CFLAGS:=${LUA_CFLAGS} ${SDL_CFLAGS}
LUASDL_LIBS:=${QKENG_LIBS} ${SDL_LIBS} -lSDL_image -lSDL_net -lSDL_ttf

# Link tolua++ statically, otherwise distribution will be a nightmare.
${LUASDL_DIR}libluasdl.so: ${LUASDL_DIR}sdllib.o ${LUASDL_DIR}SDL_bind.o ${TOLUAPP_DIR}lib/libtolua++_static.a
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${LUASDL_LIBS} $^

${LUASDL_DIR}sdllib.o: ${LUASDL_DIR}sdllib.c ${LUASDL_DIR}sdllib.h ${LUASDL_DIR}SDL_bind.h ${TOLUAPP_DIR}include/tolua++.h
	gcc -fPIC -o $@ ${LUASDL_CFLAGS} -I${TOLUAPP_DIR}include -c $<

${LUASDL_DIR}SDL_bind.o: ${LUASDL_DIR}SDL_bind.c ${LUASDL_DIR}SDL_bind.h ${TOLUAPP_DIR}include/tolua++.h
	gcc -fPIC -o $@ ${LUASDL_CFLAGS} -I${TOLUAPP_DIR}include -c $<

# These commands produce both outputs, but I'm not sure how to "combine" them in Make.
# Also, this cd's because tolua++ doesn't accept search directories.
${LUASDL_DIR}SDL_bind.c: ${TOLUAPP_DIR}bin/tolua++ ${LUASDL_DIR}pkg/SDL_config.h.pkg ${LUASDL_DIR}pkg/SDL_platform.h.pkg ${LUASDL_DIR}pkg/*.pkg
	cd ${LUASDL_DIR}pkg/ && ${LUASDL_DIR_BACK}../${TOLUAPP_DIR}bin/tolua++ -o ../SDL_bind.c -H ../SDL_bind.h SDL.pkg && cd -
${LUASDL_DIR}SDL_bind.h: ${TOLUAPP_DIR}bin/tolua++ ${LUASDL_DIR}pkg/SDL_config.h.pkg ${LUASDL_DIR}pkg/SDL_platform.h.pkg ${LUASDL_DIR}pkg/*.pkg
	cd ${LUASDL_DIR}pkg/ && ${LUASDL_DIR_BACK}../${TOLUAPP_DIR}bin/tolua++ -o ../SDL_bind.c -H ../SDL_bind.h SDL.pkg && cd -

# C preprocessor (ab)use to generate the right code for the right platform.
${LUASDL_DIR}pkg/SDL_config.h.pkg: ${LUASDL_DIR}pkg/SDL_config.h.pkg.in
	cpp -P $< $@
${LUASDL_DIR}pkg/SDL_platform.h.pkg: ${LUASDL_DIR}pkg/SDL_platform.h.pkg.in
	cpp -P -D__LINUX__ $< $@

.PHONY: clean_luasdl
clean_luasdl:
	-rm -f ${LUASDL_DIR}libluasdl.so
	-rm -f ${LUASDL_DIR}sdllib.o
	-rm -f ${LUASDL_DIR}SDL_bind.o
	-rm -f ${LUASDL_DIR}SDL_bind.c ${LUASDL_DIR}SDL_bind.h
	-rm -f ${LUASDL_DIR}pkg/SDL_config.h.pkg ${LUASDL_DIR}pkg/SDL_platform.h.pkg


### proteaAudio bindings ###

PROTEAAUDIO_CFLAGS:=-O2 -Wall -I${PROTEAAUDIO_DIR}rtaudio
PROTEAAUDIO_LIBS:=-lpthread ${ALSA_LIBS} ${LUA_LIBS}

${PROTEAAUDIO_DIR}libproaudio.so: ${PROTEAAUDIO_DIR}proAudioRt_lua.o ${PROTEAAUDIO_DIR}libproaudio.a
	g++ -shared -Wl,-soname,$(notdir $@) -o $@ ${PROTEAAUDIO_LIBS} $^

${PROTEAAUDIO_DIR}proAudioRt_lua.o: ${PROTEAAUDIO_DIR}proAudioRt_lua.cpp ${PROTEAAUDIO_DIR}proAudioRt.h
	g++ -fPIC -o $@ ${PROTEAAUDIO_CFLAGS} ${LUA_CFLAGS} -c $<

${PROTEAAUDIO_DIR}libproaudio.a: ${PROTEAAUDIO_DIR}proAudio.o ${PROTEAAUDIO_DIR}proAudioRt.o ${PROTEAAUDIO_DIR}stb_vorbis.o ${PROTEAAUDIO_DIR}rtaudio/RtAudio.o
	ar -rcs $@ $+

${PROTEAAUDIO_DIR}rtaudio/RtAudio.o: ${PROTEAAUDIO_DIR}rtaudio/RtAudio.cpp ${PROTEAAUDIO_DIR}rtaudio/RtAudio.h ${PROTEAAUDIO_DIR}rtaudio/RtError.h
	g++ -fPIC -o $@ ${PROTEAAUDIO_CFLAGS} -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__ -c $<

${PROTEAAUDIO_DIR}stb_vorbis.o: ${PROTEAAUDIO_DIR}stb_vorbis.c
	gcc -fPIC -o $@ ${PROTEAAUDIO_CFLAGS} -c $<

${PROTEAAUDIO_DIR}proAudioRt.o: ${PROTEAAUDIO_DIR}proAudioRt.cpp ${PROTEAAUDIO_DIR}proAudioRt.h ${PROTEAAUDIO_DIR}rtaudio/RtAudio.h ${PROTEAAUDIO_DIR}rtaudio/RtError.h
	g++ -fPIC -o $@ ${PROTEAAUDIO_CFLAGS} -c $<

${PROTEAAUDIO_DIR}proAudio.o: ${PROTEAAUDIO_DIR}proAudio.cpp ${PROTEAAUDIO_DIR}proAudio.h
	g++ -fPIC -o $@ ${PROTEAAUDIO_CFLAGS} -c $<

.PHONY: clean_proteaaudio
clean_proteaaudio:
	-rm -f ${PROTEAAUDIO_DIR}libproaudio.so
	-rm -f ${PROTEAAUDIO_DIR}libproaudio.a
	-rm -f ${PROTEAAUDIO_DIR}*.o
	-rm -f ${PROTEAAUDIO_DIR}rtaudio/RtAudio.o


### sdlgl helper library ##

SDLGL_CFLAGS:=${QKENG_CFLAGS} ${SDL_CFLAGS} ${GL_CFLAGS} ${GLU_CFLAGS}
SDLGL_LIBS:=${QKENG_LIBS} ${SDL_LIBS} ${GL_LIBS} ${GLU_LIBS}

${SDLGL_DIR}libsdlgl.so: ${SDLGL_DIR}sdlgl.o
	gcc -shared -Wl,-soname,$(notdir $@) -o $@ ${SDLGL_LIBS} $^

${SDLGL_DIR}sdlgl.o: ${SDLGL_DIR}sdlgl.c
	gcc -fPIC -o $@ ${SDLGL_CFLAGS} -c $<

.PHONY: clean_sdlgl
clean_sdlgl:
	-rm -f ${SDLGL_DIR}libsdlgl.so
	-rm -f ${SDLGL_DIR}sdlgl.o


### tolua++ utility ###

# All of this was based on me watching scons doing its thing.
.PHONY: all_toluapp
all_toluapp: ${TOLUAPP_DIR}lib/libtolua++.a ${TOLUAPP_DIR}lib/libtolua++_static.a ${TOLUAPP_DIR}bin/tolua++

# The steps in this feel wrong, i.e. it's how static libs are made, but scons did it, so here it is.
${TOLUAPP_DIR}lib/libtolua++.a: ${TOLUAPP_DIR}src/lib/tolua_event.o ${TOLUAPP_DIR}src/lib/tolua_is.o ${TOLUAPP_DIR}src/lib/tolua_map.o ${TOLUAPP_DIR}src/lib/tolua_push.o ${TOLUAPP_DIR}src/lib/tolua_to.o
	mkdir -p ${TOLUAPP_DIR}lib
	ar rc $@ $+
	ranlib $@

# Yeah, this and the above were identical in scons' processing.
${TOLUAPP_DIR}lib/libtolua++_static.a: ${TOLUAPP_DIR}src/lib/tolua_event.o ${TOLUAPP_DIR}src/lib/tolua_is.o ${TOLUAPP_DIR}src/lib/tolua_map.o ${TOLUAPP_DIR}src/lib/tolua_push.o ${TOLUAPP_DIR}src/lib/tolua_to.o
	mkdir -p ${TOLUAPP_DIR}lib
	ar rc $@ $+
	ranlib $@

${TOLUAPP_DIR}src/lib/tolua_event.o: ${TOLUAPP_DIR}src/lib/tolua_event.c
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<
${TOLUAPP_DIR}src/lib/tolua_is.o: ${TOLUAPP_DIR}src/lib/tolua_is.c
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<
${TOLUAPP_DIR}src/lib/tolua_map.o: ${TOLUAPP_DIR}src/lib/tolua_map.c
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<
${TOLUAPP_DIR}src/lib/tolua_push.o: ${TOLUAPP_DIR}src/lib/tolua_push.c
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<
${TOLUAPP_DIR}src/lib/tolua_to.o: ${TOLUAPP_DIR}src/lib/tolua_to.c
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<

# The tolua++ utility itself, which turns a cleaned C header into a Lua binding.
# This binding can then be compiled and used as normal, but with the tolua++ lib and headers.
${TOLUAPP_DIR}bin/tolua++: ${TOLUAPP_DIR}src/bin/tolua.o ${TOLUAPP_DIR}src/bin/toluabind.o ${TOLUAPP_DIR}lib/libtolua++.a
	mkdir -p ${TOLUAPP_DIR}bin
	gcc -o $@ ${TOLUAPP_DIR}src/bin/tolua.o ${TOLUAPP_DIR}src/bin/toluabind.o -L${TOLUAPP_DIR}lib -ltolua++ ${LUA_LIBS}

${TOLUAPP_DIR}src/bin/tolua.o: ${TOLUAPP_DIR}src/bin/tolua.c ${TOLUAPP_DIR}include/tolua++.h
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<

${TOLUAPP_DIR}src/bin/toluabind.o: ${TOLUAPP_DIR}src/bin/toluabind.c ${TOLUAPP_DIR}include/tolua++.h
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -ansi -Wall -I${TOLUAPP_DIR}include $<

# tolua++_bootstrap creates both toluabind.c AND toluabind.h.
# AFAIK there's no way to tell Make that making one makes the other too.
${TOLUAPP_DIR}src/bin/toluabind.c: ${TOLUAPP_DIR}src/bin/tolua_scons.pkg ${TOLUAPP_DIR}bin/tolua++_bootstrap ${TOLUAPP_DIR}src/bin/lua/*.lua
	cd ${TOLUAPP_DIR} && bin/tolua++_bootstrap -C -H src/bin/toluabind.h -o src/bin/toluabind.c -n tolua src/bin/tolua_scons.pkg && cd -
${TOLUAPP_DIR}src/bin/toluabind.h: ${TOLUAPP_DIR}src/bin/tolua_scons.pkg ${TOLUAPP_DIR}bin/tolua++_bootstrap ${TOLUAPP_DIR}src/bin/lua/*.lua
	cd ${TOLUAPP_DIR} && bin/tolua++_bootstrap -C -H src/bin/toluabind.h -o src/bin/toluabind.c -n tolua src/bin/tolua_scons.pkg && cd -

${TOLUAPP_DIR}bin/tolua++_bootstrap: ${TOLUAPP_DIR}src/bin/tolua.o ${TOLUAPP_DIR}src/bin/toluabind_default.o ${TOLUAPP_DIR}lib/libtolua++_static.a
	mkdir -p ${TOLUAPP_DIR}bin
	gcc -o $@ $^ -L${TOLUAPP_DIR}lib $(LUA_LIBS)

# Like toluabind.c, this looks like a generated file, but I can't find its .pkg anywhere.
# It's a shame, because it's 500 KB large in this form...
${TOLUAPP_DIR}src/bin/toluabind_default.o: ${TOLUAPP_DIR}src/bin/toluabind_default.c ${TOLUAPP_DIR}include/tolua++.h
	gcc -o $@ -c ${LUA_CFLAGS} -O2 -Wall -ansi -I${TOLUAPP_DIR}include $<

.PHONY: clean_toluapp
clean_toluapp:
	-rm -f ${TOLUAPP_DIR}lib/libtolua++.a ${TOLUAPP_DIR}lib/libtolua++_static.a
	-rm -f ${TOLUAPP_DIR}src/lib/tolua_*.o
	-rm -f ${TOLUAPP_DIR}bin/tolua++
	-rm -f ${TOLUAPP_DIR}src/bin/tolua.o
	-rm -f ${TOLUAPP_DIR}src/bin/toluabind.o
	-rm -f ${TOLUAPP_DIR}src/bin/toluabind.c
	-rm -f ${TOLUAPP_DIR}src/bin/toluabind.h
	-rm -f ${TOLUAPP_DIR}bin/tolua++_bootstrap
	-rm -f ${TOLUAPP_DIR}src/bin/toluabind_default.o
	-rmdir ${TOLUAPP_DIR}bin
	-rmdir ${TOLUAPP_DIR}lib
