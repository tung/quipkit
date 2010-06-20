# make configuration for luaSDL -Thatcher Ulrich

# Public Domain


# If you haven't set TU_TESTBED_COMPILER, then take a guess at its value
# based on the OSTYPE env variable.
#
# If you want to use a particular compiler; for example gcc-3.0, then
# set the environment variable TU_TESTBED_COMPILER to the compiler
# name.
ifndef TU_TESTBED_COMPILER
	ifndef $(OSTYPE)
		OSTYPE = $(shell uname)
	endif
	OSTYPE := $(patsubst Linux, linux, $(OSTYPE))

	ifneq (,$(findstring linux, $(OSTYPE)))
		TU_TESTBED_COMPILER = gcc
	else
		TU_TESTBED_COMPILER = msvc
	endif
endif


ifeq ($(TU_TESTBED_COMPILER), msvc)

#
# MSVC/Windows options
#


# MSVC
CC := cl /nologo
AR := lib /NOLOGO
#LINK := link /nologo
LIB_OUT_FLAG := /OUT:
LIB_EXT := lib
LIB_PRE :=
OBJ_EXT := obj
EXE_EXT := .exe
DLL_EXT := dll
SDL_LIBS := winmm.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib
LIBS := opengl32.lib glu32.lib SDL_image.lib SDL.lib gdi32.lib oldnames.lib $(SDL_LIBS)
SDL_CFLAGS := -ISDL
CFLAGS := $(CFLAGS) $(SDL_CFLAGS) /MD -GR -GX -DWIN32=1 -W3
LDFLAGS := /link /NODEFAULTLIB:libc.lib /INCREMENTAL:NO

ifeq ($(DEBUG),1)
	# msvc debug flags
	CC_DEBUG_FLAGS := -Zi -Od
	LDFLAGS := -Zi $(LDFLAGS) -DEBUG -DEBUGTYPE:BOTH /NODEFAULTLIB:msvcrt.lib msvcrtd.lib
	MAKE_DLL := cl /nologo /LDd
else
	# msvc non-debug flags
	CC_DEBUG_FLAGS := -Ox -DNDEBUG=1
	MAKE_DLL := cl /nologo /LD
endif


else


# GCC
CC := $(TU_TESTBED_COMPILER)
AR := ar -r
LIB_OUT_FLAG :=
LIB_PRE := lib
LIB_EXT := a
OBJ_EXT := o
EXE_EXT :=
DLL_EXT := so
LIBS := -lSDL_image -lSDL -lGL -lGLU -lm
DLL_LIBS := -ldl
SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)
CFLAGS := $(CFLAGS) $(SDL_CFLAGS) -fpic -Wall
LDFLAGS := -lGL -lGLU -lSDL_image
MAKE_DLL := gcc -shared

ifeq ($(DEBUG), 1)
	# gcc debug flags
	CC_DEBUG_FLAGS := -g
	LDFLAGS := -g
else
	# gcc non-debug flags
	CC_DEBUG_FLAGS := -O2 -DNDEBUG=1 -ffast-math -fexpensive-optimizations -fomit-frame-pointer
endif

endif # GCC


%.$(OBJ_EXT): %.cpp
	$(CC) -c $< $(CFLAGS)

%.$(OBJ_EXT): %.c
	$(CC) -c $< $(CFLAGS)


ENGINE := $(TOP)/engine/engine.$(LIB_EXT)
ENGINE_INCLUDE := -I$(TOP)
