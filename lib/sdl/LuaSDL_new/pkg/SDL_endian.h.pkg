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

#define SDL_LIL_ENDIAN	1234
#define SDL_BIG_ENDIAN	4321

// NOTE: Platform-dependent definitions for SDL_BYTEORDER, in tolua++
//       we cannot really do all the testing for processors etc.
//       From: SDL_endian.h, To: SDL_config_*.h

// NOTE: No real support for 64-bit ints at present, casting of 64-bit
//       ints to an IEEE double will probably fail...

// interface for macros

Uint16 SDL_Swap16 (Uint16 x);
Uint32 SDL_Swap32 (Uint32 x);
// Uint64 SDL_Swap64 (Uint64 x);

// interface of dummy swaps for LE/BE

Uint16 SDL_Pass16 (Uint16 x);
Uint32 SDL_Pass32 (Uint32 x);
// Uint64 SDL_Pass64 (Uint64 x);

// implementation of dummy swaps for LE/BE

$       inline Uint16 SDL_Pass16 (Uint16 x)
$       {
$           return x;
$       }
$       inline Uint32 SDL_Pass32 (Uint32 x)
$       {
$           return x;
$       }
// $       inline Uint64 SDL_Pass64 (Uint64 x)
// $       {
// $           return x;
// $       }

// implementation of LE/BE swap functions

$[

local SDL = SDL
if SDL.SDL_BYTEORDER == SDL.SDL_LIL_ENDIAN then
    SDL.SDL_SwapLE16 = SDL.SDL_Pass16
    SDL.SDL_SwapLE32 = SDL.SDL_Pass32
    SDL.SDL_SwapBE16 = SDL.SDL_Swap16
    SDL.SDL_SwapBE32 = SDL.SDL_Swap32
else
    SDL.SDL_SwapLE16 = SDL.SDL_Swap16
    SDL.SDL_SwapLE32 = SDL.SDL_Swap32
    SDL.SDL_SwapBE16 = SDL.SDL_Pass16
    SDL.SDL_SwapBE32 = SDL.SDL_Pass32
end

$]

// if SDL.SDL_BYTEORDER == SDL.SDL_LIL_ENDIAN then
//     SDL.SDL_SwapLE64 = SDL.SDL_Pass64
//     SDL.SDL_SwapBE64 = SDL.SDL_Swap64
// else
//     SDL.SDL_SwapLE64 = SDL.SDL_Swap64
//     SDL.SDL_SwapBE64 = SDL.SDL_Pass64
// end
