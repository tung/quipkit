/* ================================================================== */
/*
 * tolua++ bindings for LuaSDL, adapted from SDL header files.
 * This file is part of LuaSDL.
 *
 * Author: Kein-Hong Man <khman@users.sf.net> 2007
 */
/* ================================================================== */

/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

// NOTE: don't try to access the function pointers in SDL_RWops directly,
//       please use the convenience functions (which used to be macros)

typedef struct SDL_RWops {
	Uint32 type;
} SDL_RWops;

extern SDL_RWops * SDL_RWFromFile(const char *file, const char *mode);
// NOTE: implemented in sdllib.c, allowing Lua file handles to be used
// extern SDL_RWops * SDL_RWFromFP(FILE *fp, int autoclose);
extern SDL_RWops * SDL_RWFromMem(void *mem, int size);
extern SDL_RWops * SDL_RWFromConstMem(const void *mem, int size);
extern SDL_RWops * SDL_AllocRW(void);
extern void SDL_FreeRW(SDL_RWops *area);

#define RW_SEEK_SET	0
#define RW_SEEK_CUR	1
#define RW_SEEK_END	2

// interface for macros

int SDL_RWseek(struct SDL_RWops *context, int offset, int whence);
int SDL_RWtell(struct SDL_RWops *context);
int SDL_RWread(struct SDL_RWops *context, void *ptr, int size, int maxnum);
int SDL_RWwrite(struct SDL_RWops *context, const void *ptr, int size, int num);
int SDL_RWclose(struct SDL_RWops *context);

extern Uint16 SDL_ReadLE16(SDL_RWops *src);
extern Uint16 SDL_ReadBE16(SDL_RWops *src);
extern Uint32 SDL_ReadLE32(SDL_RWops *src);
extern Uint32 SDL_ReadBE32(SDL_RWops *src);

// NOTE: 64-bit integers currently unsupported, cast to double problem
// extern Uint64 SDL_ReadLE64(SDL_RWops *src);
// extern Uint64 SDL_ReadBE64(SDL_RWops *src);

extern int SDL_WriteLE16(SDL_RWops *dst, Uint16 value);
extern int SDL_WriteBE16(SDL_RWops *dst, Uint16 value);
extern int SDL_WriteLE32(SDL_RWops *dst, Uint32 value);
extern int SDL_WriteBE32(SDL_RWops *dst, Uint32 value);

// NOTE: 64-bit integers currently unsupported, cast to double problem
// extern int SDL_WriteLE64(SDL_RWops *dst, Uint64 value);
// extern int SDL_WriteBE64(SDL_RWops *dst, Uint64 value);
