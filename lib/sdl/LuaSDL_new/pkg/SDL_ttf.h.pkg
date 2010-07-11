/* ================================================================== */
/*
 * tolua++ bindings for LuaSDL, adapted from SDL header files.
 * This file is part of LuaSDL.
 *
 * Author: Kein-Hong Man <khman@users.sf.net> 2007
 */
/* ================================================================== */

/*
    SDL_ttf:  A companion library to SDL for working with TrueType (tm) fonts
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#define SDL_TTF_MAJOR_VERSION	2
#define SDL_TTF_MINOR_VERSION	0
#define SDL_TTF_PATCHLEVEL	9

// interface for macro

void SDL_TTF_VERSION (SDL_version *ver_info);

#define TTF_MAJOR_VERSION	SDL_TTF_MAJOR_VERSION
#define TTF_MINOR_VERSION	SDL_TTF_MINOR_VERSION
#define TTF_PATCHLEVEL		SDL_TTF_PATCHLEVEL

#define UNICODE_BOM_NATIVE	0xFEFF
#define UNICODE_BOM_SWAPPED	0xFFFE

extern void TTF_ByteSwappedUNICODE(int swapped);

typedef struct _TTF_Font TTF_Font;

extern int TTF_Init(void);

extern TTF_Font * TTF_OpenFont(const char *file, int ptsize);
extern TTF_Font * TTF_OpenFontIndex(const char *file, int ptsize, long index);
extern TTF_Font * TTF_OpenFontRW(SDL_RWops *src, int freesrc, int ptsize);
extern TTF_Font * TTF_OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index);

#define TTF_STYLE_NORMAL	0x00
#define TTF_STYLE_BOLD		0x01
#define TTF_STYLE_ITALIC	0x02
#define TTF_STYLE_UNDERLINE	0x04

// NOTE: tolua++ 1.0.92 cannot deal with "const TTF_Font *font"
extern int TTF_GetFontStyle(TTF_Font *font);
extern void TTF_SetFontStyle(TTF_Font *font, int style);

extern int TTF_FontHeight(TTF_Font *font);
extern int TTF_FontAscent(TTF_Font *font);
extern int TTF_FontDescent(TTF_Font *font);
extern int TTF_FontLineSkip(TTF_Font *font);
extern long TTF_FontFaces(TTF_Font *font);

extern int TTF_FontFaceIsFixedWidth(TTF_Font *font);
extern char * TTF_FontFaceFamilyName(TTF_Font *font);
extern char * TTF_FontFaceStyleName(TTF_Font *font);

extern int TTF_GlyphMetrics(TTF_Font *font, Uint16 ch,
                            int *minx = 0, int *maxx = 0,
                            int *miny = 0, int *maxy = 0, int *advance = 0);

extern int TTF_SizeText(TTF_Font *font, const char *text, int *w = 0, int *h = 0);
extern int TTF_SizeUTF8(TTF_Font *font, const char *text, int *w = 0, int *h = 0);
extern int TTF_SizeUNICODE(TTF_Font *font, const Uint16 *text, int *w = 0, int *h = 0);

extern SDL_Surface * TTF_RenderText_Solid(TTF_Font *font,
				const char *text, SDL_Color fg);
extern SDL_Surface * TTF_RenderUTF8_Solid(TTF_Font *font,
				const char *text, SDL_Color fg);
extern SDL_Surface * TTF_RenderUNICODE_Solid(TTF_Font *font,
				const Uint16 *text, SDL_Color fg);

extern SDL_Surface * TTF_RenderGlyph_Solid(TTF_Font *font,
					Uint16 ch, SDL_Color fg);

extern SDL_Surface * TTF_RenderText_Shaded(TTF_Font *font,
				const char *text, SDL_Color fg, SDL_Color bg);
extern SDL_Surface * TTF_RenderUTF8_Shaded(TTF_Font *font,
				const char *text, SDL_Color fg, SDL_Color bg);
extern SDL_Surface * TTF_RenderUNICODE_Shaded(TTF_Font *font,
				const Uint16 *text, SDL_Color fg, SDL_Color bg);

extern SDL_Surface * TTF_RenderGlyph_Shaded(TTF_Font *font,
				Uint16 ch, SDL_Color fg, SDL_Color bg);

extern SDL_Surface * TTF_RenderText_Blended(TTF_Font *font,
				const char *text, SDL_Color fg);
extern SDL_Surface * TTF_RenderUTF8_Blended(TTF_Font *font,
				const char *text, SDL_Color fg);
extern SDL_Surface * TTF_RenderUNICODE_Blended(TTF_Font *font,
				const Uint16 *text, SDL_Color fg);

extern SDL_Surface * TTF_RenderGlyph_Blended(TTF_Font *font,
						Uint16 ch, SDL_Color fg);

extern void TTF_CloseFont(TTF_Font *font);
extern void TTF_Quit(void);
extern int TTF_WasInit(void);

$[

local SDL = SDL
SDL.TTF_VERSION = SDL.SDL_TTF_VERSION

SDL.TTF_RenderText = SDL.TTF_RenderText_Shaded
SDL.TTF_RenderUTF8 = SDL.TTF_RenderUTF8_Shaded
SDL.TTF_RenderUNICODE = SDL.TTF_RenderUNICODE_Shaded

SDL.TTF_SetError = SDL.SDL_SetError
SDL.TTF_GetError = SDL.SDL_GetError

$]
