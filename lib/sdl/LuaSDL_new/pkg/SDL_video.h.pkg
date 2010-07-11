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

#define SDL_ALPHA_OPAQUE 255
#define SDL_ALPHA_TRANSPARENT 0

typedef struct SDL_Rect {
	Sint16 x, y;
	Uint16 w, h;
} SDL_Rect;

typedef struct SDL_Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 unused;
} SDL_Color;

// NOTE: I use UK English myself, but it's better to stick to one definition
//       in Lua code. If anyone has a good aliasing method, please fix this.
// #define SDL_Colour SDL_Color

typedef struct SDL_Palette {
	int       ncolors;
	SDL_Color colors[]; // index to palette colours
} SDL_Palette;

typedef struct SDL_PixelFormat {
	SDL_Palette *palette;
	Uint8  BitsPerPixel;
	Uint8  BytesPerPixel;
	Uint8  Rloss;
	Uint8  Gloss;
	Uint8  Bloss;
	Uint8  Aloss;
	Uint8  Rshift;
	Uint8  Gshift;
	Uint8  Bshift;
	Uint8  Ashift;
	Uint32 Rmask;
	Uint32 Gmask;
	Uint32 Bmask;
	Uint32 Amask;
	Uint32 colorkey;
	Uint8  alpha;
} SDL_PixelFormat;

// all read-only except pixels
typedef struct SDL_Surface {
	Uint32 flags;
	SDL_PixelFormat *format;
	int w, h;
	Uint16 pitch;
        // NOTE: for direct access, lock surface if needed and use
        //       supplemental functions SDL_GetPixel and SDL_PutPixel
	void *pixels; // read/write
	SDL_Rect clip_rect;
	Uint32 unused1;
	int refcount;
} SDL_Surface;

#define SDL_SWSURFACE	0x00000000
#define SDL_HWSURFACE	0x00000001
#define SDL_ASYNCBLIT	0x00000004
#define SDL_ANYFORMAT	0x10000000
#define SDL_HWPALETTE	0x20000000
#define SDL_DOUBLEBUF	0x40000000
#define SDL_FULLSCREEN	0x80000000
#define SDL_OPENGL      0x00000002
#define SDL_OPENGLBLIT	0x0000000A
#define SDL_RESIZABLE	0x00000010
#define SDL_NOFRAME	0x00000020
#define SDL_HWACCEL	0x00000100
#define SDL_SRCCOLORKEY	0x00001000
#define SDL_RLEACCELOK	0x00002000
#define SDL_RLEACCEL	0x00004000
#define SDL_SRCALPHA	0x00010000
#define SDL_PREALLOC	0x01000000

// interface of macro

SDL_bool SDL_MUSTLOCK(SDL_Surface * surface);

// NOTE: No real need for this yet. Probably better left as private.
// typedef int (*SDL_blit)(struct SDL_Surface *src, SDL_Rect *srcrect,
// 			struct SDL_Surface *dst, SDL_Rect *dstrect);

typedef struct SDL_VideoInfo {
	Uint32 hw_available;
	Uint32 wm_available;
	Uint32 UnusedBits1;
	Uint32 UnusedBits2;
	Uint32 blit_hw;
	Uint32 blit_hw_CC;
	Uint32 blit_hw_A;
	Uint32 blit_sw;
	Uint32 blit_sw_CC;
	Uint32 blit_sw_A;
	Uint32 blit_fill;
	Uint32 UnusedBits3;
	Uint32 video_mem;
	SDL_PixelFormat *vfmt;
	int    current_w;
	int    current_h;
} SDL_VideoInfo;

#define SDL_YV12_OVERLAY  0x32315659
#define SDL_IYUV_OVERLAY  0x56555949
#define SDL_YUY2_OVERLAY  0x32595559
#define SDL_UYVY_OVERLAY  0x59565955
#define SDL_YVYU_OVERLAY  0x55595659

typedef struct SDL_Overlay {
	Uint32 format;
	int w, h;
	int planes;
	Uint16 pitches[]; // Uint16 *
	void *pixels[]; // Uint8 **
	Uint32 hw_overlay;
	Uint32 UnusedBits;
} SDL_Overlay;

typedef enum {
    SDL_GL_RED_SIZE,
    SDL_GL_GREEN_SIZE,
    SDL_GL_BLUE_SIZE,
    SDL_GL_ALPHA_SIZE,
    SDL_GL_BUFFER_SIZE,
    SDL_GL_DOUBLEBUFFER,
    SDL_GL_DEPTH_SIZE,
    SDL_GL_STENCIL_SIZE,
    SDL_GL_ACCUM_RED_SIZE,
    SDL_GL_ACCUM_GREEN_SIZE,
    SDL_GL_ACCUM_BLUE_SIZE,
    SDL_GL_ACCUM_ALPHA_SIZE,
    SDL_GL_STEREO,
    SDL_GL_MULTISAMPLEBUFFERS,
    SDL_GL_MULTISAMPLESAMPLES,
    SDL_GL_ACCELERATED_VISUAL,
    SDL_GL_SWAP_CONTROL
} SDL_GLattr;

#define SDL_LOGPAL 0x01
#define SDL_PHYSPAL 0x02

extern char * SDL_VideoDriverName(char *namebuf, int maxlen);
extern SDL_Surface * SDL_GetVideoSurface(void);
extern const SDL_VideoInfo * SDL_GetVideoInfo(void);
extern int SDL_VideoModeOK(int width, int height, int bpp, Uint32 flags);
// NOTE: implemented in sdllib.c, returning SDL_Rect[]
// extern SDL_Rect ** SDL_ListModes(SDL_PixelFormat *format, Uint32 flags);
extern SDL_Surface * SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags);
// NOTE: implemented in sdllib.c, see comment there
// extern void SDL_UpdateRects(SDL_Surface *screen, int numrects, SDL_Rect *rects);
extern void SDL_UpdateRect(SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h);
extern int SDL_Flip(SDL_Surface *screen);

extern int SDL_SetGamma(float red, float green, float blue);
extern int SDL_SetGammaRamp(const Uint16 *red, const Uint16 *green, const Uint16 *blue);
extern int SDL_GetGammaRamp(Uint16 *red, Uint16 *green, Uint16 *blue);
extern int SDL_SetColors(SDL_Surface *surface,
			SDL_Color *colors, int firstcolor, int ncolors);
extern int SDL_SetPalette(SDL_Surface *surface, int flags,
				   SDL_Color *colors, int firstcolor,
				   int ncolors);

// NOTE: tolua++ 1.0.92 borks on "const SDL_PixelFormat * const format"
extern Uint32 SDL_MapRGB(const SDL_PixelFormat * format,
                         const Uint8 r, const Uint8 g, const Uint8 b);
extern Uint32 SDL_MapRGBA(const SDL_PixelFormat * format,
                          const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);

extern void SDL_GetRGB(Uint32 pixel, SDL_PixelFormat *fmt,
				Uint8 *r, Uint8 *g, Uint8 *b);
extern void SDL_GetRGBA(Uint32 pixel, SDL_PixelFormat *fmt,
				 Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);

extern SDL_Surface * SDL_CreateRGBSurface
			(Uint32 flags, int width, int height, int depth,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
extern SDL_Surface * SDL_CreateRGBSurfaceFrom(void *pixels,
			int width, int height, int depth, int pitch,
			Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
extern void SDL_FreeSurface(SDL_Surface *surface);

extern int SDL_LockSurface(SDL_Surface *surface);
extern void SDL_UnlockSurface(SDL_Surface *surface);
extern SDL_Surface * SDL_LoadBMP_RW(SDL_RWops *src, int freesrc);
extern int SDL_SaveBMP_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst);

// interface of macros

SDL_Surface * SDL_LoadBMP(const char *file);
int SDL_SaveBMP(SDL_Surface *surface, const char *file);

extern int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key);
extern int SDL_SetAlpha(SDL_Surface *surface, Uint32 flag, Uint8 alpha);
extern SDL_bool SDL_SetClipRect(SDL_Surface *surface, const SDL_Rect *rect);
extern void SDL_GetClipRect(SDL_Surface *surface, SDL_Rect *rect);
extern SDL_Surface * SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, Uint32 flags);

extern int SDL_UpperBlit(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);
extern int SDL_LowerBlit(SDL_Surface *src, SDL_Rect *srcrect,
			 SDL_Surface *dst, SDL_Rect *dstrect);
extern int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);

extern SDL_Surface * SDL_DisplayFormat(SDL_Surface *surface);
extern SDL_Surface * SDL_DisplayFormatAlpha(SDL_Surface *surface);

extern SDL_Overlay * SDL_CreateYUVOverlay(int width, int height,
				Uint32 format, SDL_Surface *display);
extern int SDL_LockYUVOverlay(SDL_Overlay *overlay);
extern void SDL_UnlockYUVOverlay(SDL_Overlay *overlay);
extern int SDL_DisplayYUVOverlay(SDL_Overlay *overlay, SDL_Rect *dstrect);
extern void SDL_FreeYUVOverlay(SDL_Overlay *overlay);

extern int SDL_GL_LoadLibrary(const char *path);
extern void * SDL_GL_GetProcAddress(const char* proc);
extern int SDL_GL_SetAttribute(SDL_GLattr attr, int value);
extern int SDL_GL_GetAttribute(SDL_GLattr attr, int* value);
extern void SDL_GL_SwapBuffers(void);
extern void SDL_GL_UpdateRects(int numrects, SDL_Rect* rects);
extern void SDL_GL_Lock(void);
extern void SDL_GL_Unlock(void);

extern void SDL_WM_SetCaption(const char *title, const char *icon);
extern void SDL_WM_GetCaption(char **title, char **icon);
extern void SDL_WM_SetIcon(SDL_Surface *icon, Uint8 *mask);
extern int SDL_WM_IconifyWindow(void);
extern int SDL_WM_ToggleFullScreen(SDL_Surface *surface);

typedef enum {
	SDL_GRAB_QUERY = -1,
	SDL_GRAB_OFF = 0,
	SDL_GRAB_ON = 1,
	SDL_GRAB_FULLSCREEN
} SDL_GrabMode;

extern SDL_GrabMode SDL_WM_GrabInput(SDL_GrabMode mode);

$[

SDL.SDL_AllocSurface = SDL.SDL_CreateRGBSurface
SDL.SDL_BlitSurface = SDL.SDL_UpperBlit

$]
