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

typedef struct SDL_AudioSpec {
	int freq;
	Uint16 format;
	Uint8  channels;
	Uint8  silence;
	Uint16 samples;
	Uint16 padding;
	Uint32 size;
        // NOTE: this is a callback, using void* in tolua++, for LuaSDL,
        //       always set to LuaSDL's SDL.LuaSDL_AudioCallback handler,
        //       unless you provide your own thread-safe C function...
	// void (SDLCALL *callback)(void *userdata, Uint8 *stream, int len);
        void *callback;
	void *userdata;
} SDL_AudioSpec;

#define AUDIO_U8	0x0008
#define AUDIO_S8	0x8008
#define AUDIO_U16LSB	0x0010
#define AUDIO_S16LSB	0x8010
#define AUDIO_U16MSB	0x1010
#define AUDIO_S16MSB	0x9010
#define AUDIO_U16	AUDIO_U16LSB
#define AUDIO_S16	AUDIO_S16LSB

typedef struct SDL_AudioCVT {
	int needed;
	Uint16 src_format;
	Uint16 dst_format;
	double rate_incr;
	void * buf; // Uint8 *
	int    len;
	int    len_cvt;
	int    len_mult;
	double len_ratio;
        // NOTE: many fields are really private, so no need to expose them
} SDL_AudioCVT;

extern char * SDL_AudioDriverName(char *namebuf, int maxlen);
extern int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained = NULL);

typedef enum {
	SDL_AUDIO_STOPPED = 0,
	SDL_AUDIO_PLAYING,
	SDL_AUDIO_PAUSED
} SDL_audiostatus;

extern SDL_audiostatus SDL_GetAudioStatus(void);
extern void SDL_PauseAudio(int pause_on);

// NOTE: wrapped in sdllib.c to handle casting of audio_buf
extern SDL_AudioSpec * _SDL_LoadWAV_RW @ _SDL_LoadWAV_RW(SDL_RWops *src,
    int freesrc, SDL_AudioSpec *spec, void **audio_buf, Uint32 *audio_len);

// interface for macros

SDL_AudioSpec * _SDL_LoadWAV @ SDL_LoadWAV(const char *file, SDL_AudioSpec *spec,
    void **audio_buf, Uint32 *audio_len);

// NOTE: audio_buf was Uint8*, tolua++ 1.0.92 can't handle Uint8*
extern void SDL_FreeWAV(void *audio_buf);
extern int SDL_BuildAudioCVT(SDL_AudioCVT *cvt,
		Uint16 src_format, Uint8 src_channels, int src_rate,
		Uint16 dst_format, Uint8 dst_channels, int dst_rate);
extern int SDL_ConvertAudio(SDL_AudioCVT *cvt);

#define SDL_MIX_MAXVOLUME 128

// NOTE: dst, src was Uint8*, tolua++ 1.0.92 can't handle Uint8*
extern void SDL_MixAudio(void *dst, const void *src, Uint32 len, int volume);
extern void SDL_LockAudio(void);
extern void SDL_UnlockAudio(void);
extern void SDL_CloseAudio(void);

$[

if SDL.SDL_BYTEORDER == SDL.SDL_LIL_ENDIAN then
    SDL.AUDIO_U16SYS = SDL.AUDIO_U16LSB
    SDL.AUDIO_S16SYS = SDL.AUDIO_S16LSB
else
    SDL.AUDIO_U16SYS = SDL.AUDIO_U16MSB
    SDL.AUDIO_S16SYS = SDL.AUDIO_S16MSB
end

$]
