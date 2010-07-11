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

// NOTE: dummy struct for WMcursor
struct WMcursor;

typedef struct SDL_Cursor {
	SDL_Rect area;
	Sint16 hot_x, hot_y;
	void * data; // Uint8 *
	void * mask; // Uint8 *
        // NOTE: private; there is no real need to access this
        // Uint8 *save[2];
	WMcursor *wm_cursor;
} SDL_Cursor;

extern Uint8 SDL_GetMouseState(int *x = 0, int *y = 0);
extern Uint8 SDL_GetRelativeMouseState(int *x = 0, int *y = 0);
extern void SDL_WarpMouse(Uint16 x, Uint16 y);
// NOTE: data and mask originally Uint8*
extern SDL_Cursor * SDL_CreateCursor
		(void *data, void *mask, int w, int h, int hot_x, int hot_y);
extern void SDL_SetCursor(SDL_Cursor *cursor);
extern SDL_Cursor * SDL_GetCursor(void);
extern void SDL_FreeCursor(SDL_Cursor *cursor);
extern int SDL_ShowCursor(int toggle);

#define SDL_BUTTON_LEFT		1
#define SDL_BUTTON_MIDDLE	2
#define SDL_BUTTON_RIGHT	3
#define SDL_BUTTON_WHEELUP	4
#define SDL_BUTTON_WHEELDOWN	5
#define SDL_BUTTON_LMASK	SDL_BUTTON(SDL_BUTTON_LEFT)
#define SDL_BUTTON_MMASK	SDL_BUTTON(SDL_BUTTON_MIDDLE)
#define SDL_BUTTON_RMASK	SDL_BUTTON(SDL_BUTTON_RIGHT)
