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

#define SDL_TIMESLICE		10
#define TIMER_RESOLUTION	10

extern Uint32 SDL_GetTicks(void);
extern void SDL_Delay(Uint32 ms);

// NOTE: this is the old API call, won't be supported
/* extern int SDL_SetTimer(Uint32 interval, SDL_TimerCallback callback); */

// NOTE: dummy struct
// WAS: typedef struct _SDL_TimerID *SDL_TimerID;
struct _SDL_TimerID {
};

// NOTE: implemented in sdllib.c, call in Lua is not the same as
//       the original SDL call, which is defined as:
/* extern _SDL_TimerID* SDL_AddTimer(Uint32 interval, SDL_NewTimerCallback callback, void *param); */
extern struct _SDL_TimerID* _SDL_AddTimer @ SDL_AddTimer(Uint32 interval, SDL_Event *param);
extern SDL_bool SDL_RemoveTimer(struct _SDL_TimerID* t);
