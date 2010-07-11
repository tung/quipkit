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

/* In order to use these functions, SDL_Init() must have been called
   with the SDL_INIT_JOYSTICK flag.  This causes SDL to scan the system
   for joysticks, and load appropriate drivers.
*/

/* The joystick structure used to identify an SDL joystick */
struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

extern int SDL_NumJoysticks(void);
extern const char * SDL_JoystickName(int device_index);
extern SDL_Joystick * SDL_JoystickOpen(int device_index);
extern int SDL_JoystickOpened(int device_index);
extern int SDL_JoystickIndex(SDL_Joystick *joystick);
extern int SDL_JoystickNumAxes(SDL_Joystick *joystick);
extern int SDL_JoystickNumBalls(SDL_Joystick *joystick);
extern int SDL_JoystickNumHats(SDL_Joystick *joystick);
extern int SDL_JoystickNumButtons(SDL_Joystick *joystick);
extern void SDL_JoystickUpdate(void);
extern int SDL_JoystickEventState(int state);
extern Sint16 SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis);

#define SDL_HAT_CENTERED	0x00
#define SDL_HAT_UP		0x01
#define SDL_HAT_RIGHT		0x02
#define SDL_HAT_DOWN		0x04
#define SDL_HAT_LEFT		0x08
#define SDL_HAT_RIGHTUP		(SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN	(SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP		(SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN	(SDL_HAT_LEFT|SDL_HAT_DOWN)

extern Uint8 SDL_JoystickGetHat(SDL_Joystick *joystick, int hat);
extern int SDL_JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx = 0, int *dy = 0);
extern Uint8 SDL_JoystickGetButton(SDL_Joystick *joystick, int button);
extern void SDL_JoystickClose(SDL_Joystick *joystick);
