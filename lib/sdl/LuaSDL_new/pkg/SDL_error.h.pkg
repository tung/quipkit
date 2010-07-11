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

// NOTE: SDL_SetError is wrapped in a Lua function to allow multiple args
//       * fmt must conform to string.format conventions
// Use as: extern void SDL_SetError(const char *fmt, ...);
extern void SDL_SetError @ _SDL_SetError (const char *msg);

extern char * SDL_GetError(void);
extern void SDL_ClearError(void);

// implementation of SDL_SetError wrapper

$[

local SDL = SDL
SDL.SDL_SetError = function(fmt, ...)
    SDL._SDL_SetError(string.format(fmt, ...))
end

$]
