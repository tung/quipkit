------------------------------------------------------------------------
--
-- sfont.lua
-- A factory for sfont.
--
-- See the licensing information below.
-- Ported from SFont.c. Author: Kein-Hong Man <khman@users.sf.net> 2007
-- Module'd and OO'd by Tung Nguyen <tunginobi@gmail.com> 2010
--
------------------------------------------------------------------------
--
--  SFont: a simple font-library that uses special .pngs as fonts
--  Copyright (C) 2003 Karl Bartel
--
--  License: GPL or LGPL (at your choice)
--  WWW: http://www.linux-games.com/sfont/
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
--
--  Karl Bartel
--  Cecilienstr. 14
--  12307 Berlin
--  GERMANY
--  karlb@gmx.net
--
------------------------------------------------------------------------

require "sdl"

module(..., package.seeall)

------------------------------------------------------------------------
-- initialize an SFont instance from a SFont-type font image surface
------------------------------------------------------------------------
local function SFont_InitFont(Surface)
  if not Surface then
    return nil
  end

  local Font = {}
  Font.CharPos = {}
  Font.Surface = Surface

  SDL.SDL_LockSurface(Surface)

  local pink = SDL.SDL_MapRGB(Surface.format, 255, 0, 255)
  local i, x = 0, 0
  while x < Surface.w do
    if SDL.SDL_GetPixel(Surface, x, 0) == pink then
      Font.CharPos[i] = x
      i = i + 1
      while x < Surface.w and SDL.SDL_GetPixel(Surface, x, 0) == pink do
          x = x + 1
      end
      Font.CharPos[i] = x
      i = i + 1
    end
    x = x + 1
  end
  Font.MaxPos = x - 1

  local pixel = SDL.SDL_GetPixel(Surface, 0, Surface.h - 1)
  SDL.SDL_UnlockSurface(Surface)
  SDL.SDL_SetColorKey(Surface, SDL.SDL_SRCCOLORKEY, pixel)

  return Font
end

------------------------------------------------------------------------
-- frees memory in C; not required in Lua
------------------------------------------------------------------------
--[[
local function SFont_FreeFont(FontInfo)
  -- nothing to do; call kept for completeness' sake
end
--]]

------------------------------------------------------------------------
-- writes a segment of text to given surface
------------------------------------------------------------------------
local function SFont_Write(Surface, Font, x, y, text)
  local srcrect = SDL.SDL_Rect_local()
  local dstrect = SDL.SDL_Rect_local()

  if not text then return end

  -- these values won't change in the loop
  srcrect.y = 1
  dstrect.y = y
  srcrect.h = Font.Surface.h - 1
  dstrect.h = srcrect.h

  for i = 1, #text do
    if x > Surface.w then break end
    local c = string.byte(text, i)
    local charoffset = (c - 33) * 2 + 1

    -- skip spaces and nonprintable characters
    if c == 32 or charoffset < 0 or charoffset > Font.MaxPos then
      x = x + Font.CharPos[2] - Font.CharPos[1]
    else
    -- printable character, do the blit
      srcrect.w = math.floor((Font.CharPos[charoffset+2] + Font.CharPos[charoffset+1])/2) -
                  math.floor((Font.CharPos[charoffset] + Font.CharPos[charoffset-1])/2)
      dstrect.w = srcrect.w
      srcrect.x = math.floor((Font.CharPos[charoffset] + Font.CharPos[charoffset-1])/2)
      dstrect.x = x - (Font.CharPos[charoffset] - Font.CharPos[charoffset-1])/2

      SDL.SDL_BlitSurface(Font.Surface, srcrect, Surface, dstrect)

      x = x + Font.CharPos[charoffset+1] - Font.CharPos[charoffset]
    end
  end
end

------------------------------------------------------------------------
-- returns width of a segment of text
------------------------------------------------------------------------
local function SFont_TextWidth(Font, text)
  local width = 0
  if not text then return end

  for i = 1, #text do
    local c = string.byte(text, i)
    local charoffset = (c - 33) * 2 + 1

    -- skip spaces and nonprintable characters
    if c == 32 or charoffset < 0 or charoffset > Font.MaxPos then
      width = width + Font.CharPos[2] - Font.CharPos[1]
    else
      width = width + Font.CharPos[charoffset+1] - Font.CharPos[charoffset]
    end
  end

  return width
end

------------------------------------------------------------------------
-- returns height of the font
------------------------------------------------------------------------
local function SFont_TextHeight(Font)
  return Font.Surface.h - 1
end

------------------------------------------------------------------------
-- writes a segment of centered text on a given surface
------------------------------------------------------------------------
local function SFont_WriteCenter(Surface, Font, y, text)
  SFont_Write(Surface, Font,
              math.floor(Surface.w/2) - math.floor(SFont_TextWidth(Font, text)/2),
              y, text)
end

------------------------------------------------------------------------
-- SFont Lua object interface.
------------------------------------------------------------------------

function new(self, surface)
    local f = SFont_InitFont(surface)
    if not f then
        error("sfont:new failed: No surface given.", 2)
    end
    setmetatable(f, self)
    self.__index = self
    return f
end

function write(self, surface, x, y, text)
    return SFont_Write(surface, self, x, y, text)
end

-- args: self, text
textWidth = SFont_TextWidth

-- args: self
height = SFont_TextHeight

function writeCenter(self, surface, y, text)
    return SFont_WriteCenter(surface, self, y, text)
end
