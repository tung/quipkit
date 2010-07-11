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

#define SDL_MAX_TRACKS	99
#define SDL_AUDIO_TRACK	0x00
#define SDL_DATA_TRACK	0x04

typedef enum {
	CD_TRAYEMPTY,
	CD_STOPPED,
	CD_PLAYING,
	CD_PAUSED,
	CD_ERROR = -1
} CDstatus;

typedef struct SDL_CDtrack {
	Uint8 id;
	Uint8 type;
	Uint16 unused;
	Uint32 length;
	Uint32 offset;
} SDL_CDtrack;

typedef struct SDL_CD {
	int id;
	CDstatus status;
	int numtracks;
	int cur_track;
	int cur_frame;
	SDL_CDtrack track[];
} SDL_CD;

// interface for macros

SDL_bool CD_INDRIVE(CDstatus status);
void FRAMES_TO_MSF(int frames, int* min = 0, int* sec = 0, int* fr = 0);
int MSF_TO_FRAMES(int min, int sec, int frac);

#define CD_FPS	75

extern int SDL_CDNumDrives(void);
extern const char * SDL_CDName(int drive);
extern SDL_CD * SDL_CDOpen(int drive);
extern CDstatus SDL_CDStatus(SDL_CD *cdrom);
extern int SDL_CDPlayTracks(SDL_CD *cdrom,
		int start_track, int start_frame, int ntracks, int nframes);
extern int SDL_CDPlay(SDL_CD *cdrom, int start, int length);
extern int SDL_CDPause(SDL_CD *cdrom);
extern int SDL_CDResume(SDL_CD *cdrom);
extern int SDL_CDStop(SDL_CD *cdrom);
extern int SDL_CDEject(SDL_CD *cdrom);
extern void SDL_CDClose(SDL_CD *cdrom);
