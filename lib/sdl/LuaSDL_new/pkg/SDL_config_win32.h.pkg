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

// NOTE: Edited for Win32/i386 ONLY. Not for _WIN32_WCE as well.

// NOTE: Platform-dependent specification of primitive data types.
typedef signed char             int8_t;
typedef unsigned char           uint8_t;
typedef signed short            int16_t;
typedef unsigned short          uint16_t;
typedef signed int              int32_t;
typedef unsigned int            uint32_t;
typedef signed long long        int64_t;
typedef unsigned long long      uint64_t;
typedef unsigned int            size_t;
typedef unsigned int            uintptr_t;

// NOTE: Resolution of primitive types. Platform independent but tolua++
//       needs to resolve them so that the functions below can use them.
//       From: SDL_stdinc.h, To: SDL_config_*.h
typedef int8_t		Sint8;
typedef uint8_t		Uint8;
typedef int16_t		Sint16;
typedef uint16_t	Uint16;
typedef int32_t		Sint32;
typedef uint32_t	Uint32;
typedef int64_t	        Sint64;
typedef uint64_t	Uint64;

// NOTE: Only constants relevant to SDL (SDL_*) are exposed.

#define SDL_AUDIO_DRIVER_DSOUND	        1
#define SDL_AUDIO_DRIVER_WAVEOUT	1
#define SDL_AUDIO_DRIVER_DISK	        1
#define SDL_AUDIO_DRIVER_DUMMY	        1
#define SDL_CDROM_WIN32		        1
#define SDL_JOYSTICK_WINMM	        1
#define SDL_LOADSO_WIN32	        1
#define SDL_THREAD_WIN32	        1
#define SDL_TIMER_WIN32	                1
#define SDL_VIDEO_DRIVER_DDRAW	        1
#define SDL_VIDEO_DRIVER_DUMMY	        1
#define SDL_VIDEO_DRIVER_WINDIB	        1
#define SDL_VIDEO_OPENGL	        1
#define SDL_VIDEO_OPENGL_WGL	        1
#define SDL_ASSEMBLY_ROUTINES	        1

// NOTE: Platform-dependent definitions for SDL_BYTEORDER
//       From: SDL_endian.h, To: SDL_config_*.h

#define SDL_BYTEORDER	SDL_LIL_ENDIAN
