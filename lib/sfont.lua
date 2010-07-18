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

-----------------------------------------------------------------------
-- SFont SDL convenience object: no need to load a surface yourself.
-----------------------------------------------------------------------

sdlFont = {}

function sdlFont:new(image_file)
    -- Assume tolua++ has IMG_Load rigged to return a GC'able surface.
    local s = SDL.IMG_Load(image_file)
    if not s then error("IMG_Load failed: " .. SDL.IMG_GetError()) end
    local f = SFont_InitFont(s)
    if not f then error("SFont_InitFont failed: Unknown error.") end
    setmetatable(f, self)
    self.__index = self
    return f
end

function sdlFont:write(surface, x, y, text)
    return SFont_Write(surface, self, x, y, text)
end

-- args: self, text
sdlFont.textWidth = SFont_TextWidth

-- args: self
sdlFont.height = SFont_TextHeight

function sdlFont:writeCenter(self, surface, y, text)
    return SFont_WriteCenter(surface, self, y, text)
end

------------------------------------------------------------------------
-- SFont OpenGL convenience object: Load and draw as an OpenGL texture.
------------------------------------------------------------------------

glFont = {}

function glFont:new(image_file)
    -- Assume tolua++ has IMG_Load rigged to return a GC'able surface.
    local s = SDL.IMG_Load(image_file)
    if not s then error("IMG_Load failed: " .. SDL.IMG_GetError()) end
    local f = SFont_InitFont(s)
    if not f then error("SFont_InitFont failed: Unknown error.") end

    -- Rework all glyphs into a smaller 2D space so it fits in a texture.
    f.TexId = 0
    f.TexW = 0
    f.TexH = 0
    f.Height = f.Surface.h - 1  -- Don't draw the top line.

    -- Parallel arrays of characters, 1-based.
    f.CharWidth = {}        -- Width of final character displayed.
    f.GlyphWidth = {}       -- Width of the full glyph in the texture.
    f.CharGlyphOffset = {}  -- Gap between left edges of char and glyph.
    f.GlyphX = {}           -- Glyph x coordinate.
    f.GlyphY = {}           -- Glyph y coordinate.

    -- Rework CharPos data into char and glyph widths, and their offsets.
    -- CharPos[0] = pink, CharPos[1] = glyph, CharPos[2] = pink...
    do
        local x, y = 0, 0
        local max_width = 0
        local max_height = 0
        local char_in_row, max_chars_per_row = 1, 15
        local char = 1
        local charpos = f.CharPos
        for i = 1, #charpos - 2, 2 do
            local char_left = charpos[i]
            local char_right = charpos[i + 1]
            local glyph_left = math.floor((charpos[i - 1] + charpos[i]) / 2)
            local glyph_right = math.floor((charpos[i + 1] + charpos[i + 2]) / 2)

            f.CharWidth[char] = char_right - char_left
            f.GlyphWidth[char] = glyph_right - glyph_left
            f.CharGlyphOffset[char] = char_left - glyph_left

            -- Lay out glyphs ahead of time.
            f.GlyphX[char] = x
            f.GlyphY[char] = y
            if x + (glyph_right - glyph_left) > max_width then
                max_width = x + (glyph_right - glyph_left)
            end
            x = x + (glyph_right - glyph_left)

            -- Work glpyhs into rows.
            char_in_row = char_in_row + 1
            if char_in_row >= max_chars_per_row then
                char_in_row = 1
                x = 0
                y = y + f.Height
            end

            char = char + 1
        end

        -- Figure out height texture needs to accommodate.
        if x == 0 then
            max_height = y
        else
            max_height = y + f.Height
        end

        -- Create intermediate surface to send to OpenGL.
        local log2 = math.log(2)
        f.TexW = math.pow(2, math.ceil(math.log(max_width) / log2))
        f.TexH = math.pow(2, math.ceil(math.log(max_height) / log2))
        local rmask, gmask, bmask, amask
        if SDL.SDL_BYTEORDER == SDL.SDL_LIL_ENDIAN then
            rmask = 0x000000ff
            gmask = 0x0000ff00
            bmask = 0x00ff0000
            amask = 0xff000000
        else
            rmask = 0xff000000
            gmask = 0x00ff0000
            bmask = 0x0000ff00
            amask = 0x000000ff
        end
        local surface = SDL.SDL_CreateRGBSurface(SDL.SDL_SWSURFACE, f.TexW, f.TexH, 32, rmask, gmask, bmask, amask)
        if not surface then
            error("SDL_CreateRGBSurface failed: " .. SDL.SDL_GetError())
        end

        -- Transfer the glyphs from the old surface to the new one.
        local src_rect = SDL.SDL_Rect_local()
        src_rect.y = 1
        src_rect.h = f.Height
        local dest_rect = SDL.SDL_Rect_local()
        for i = 1, #(f.GlyphWidth) do
            src_rect.x = math.floor((f.CharPos[(i - 1) * 2] + f.CharPos[(i - 1) * 2 + 1]) / 2)
            src_rect.w = f.GlyphWidth[i]
            dest_rect.x = f.GlyphX[i]
            dest_rect.y = f.GlyphY[i]
            if SDL.SDL_BlitSurface(s, src_rect, surface, dest_rect) ~= 0 then
                error("SDL_BlitSurface failed for char " .. i .. ": " .. SDL.SDL_GetError())
            end
            src_rect.x = src_rect.x + src_rect.w
        end

        -- DEBUG
        --[[
        do
            SDL.SDL_LockSurface(surface)
            print(SDL.SDL_GetRGBA(SDL.SDL_GetPixel(surface, 3, 5), surface.format, 1, 1, 1, 1))
            SDL.SDL_UnlockSurface(surface)
            error("TODO: Manually port alpha values so alpha blending works.")
            -- Once done, re-enable alpha blending in game.lua.
        end
        --]]

        -- Load the new surface as an OpenGL texture.
        f.TexId = gl.GenTextures(1)[1]
        gl.BindTexture(gl.TEXTURE_2D, f.TexId)
        gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
        gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
        gl.TexImage2D(0, gl.RGBA, surface.w, surface.h, 0, gl.RGBA, gl.UNSIGNED_BYTE, surface.pixels)

        -- Free the intermediate surface.
        SDL.SDL_FreeSurface(surface)
    end

    -- Free the old font surface.
    f.Surface = nil
    SDL.SDL_FreeSurface(s)

    setmetatable(f, self)
    self.__index = self
    return f
end

function glFont:write(x, y, text)
    if not text then return end

    -- Use replace mode when drawing textures.
    gl.TexEnv(gl.TEXTURE_ENV, gl.TEXTURE_ENV_MODE, gl.REPLACE)

    gl.PushMatrix()
        -- Move the text where it should begin.
        gl.LoadIdentity()
        gl.Translate(x, y, 0)

        -- Draw glyphs as textured quads.
        gl.Enable(gl.TEXTURE_2D)
            gl.BlendFunc(gl.SRC_ALPHA, gl.ONE)
            gl.BindTexture(gl.TEXTURE_2D, self.TexId)
            gl.Begin(gl.QUADS)
                local height = self.Height
                for i = 1, #text do
                    local co = string.byte(text, i) - 32

                    if co < 1 or co > #(self.CharWidth) then
                        -- Skip spaces and non-printables.
                        x = x + self.CharWidth[1]
                    else
                        -- Map glyph metrics to texture.
                        local t_left = self.GlyphX[co] / self.TexW
                        local t_right = (self.GlyphX[co] + self.GlyphWidth[co]) / self.TexW
                        local t_top = self.GlyphY[co] / self.TexH
                        local t_bottom = (self.GlyphY[co] + self.Height) / self.TexH

                        -- Draw that glyph!
                        local off = self.CharGlyphOffset[co]
                        local width = self.GlyphWidth[co]
                        gl.TexCoord(t_left,  t_top);    gl.Vertex(x - off, 0)
                        gl.TexCoord(t_right, t_top);    gl.Vertex(x - off + width, 0)
                        gl.TexCoord(t_right, t_bottom); gl.Vertex(x - off + width, height)
                        gl.TexCoord(t_left,  t_bottom); gl.Vertex(x - off, height)

                        x = x + self.CharWidth[co]
                    end
                end
            gl.End()
        gl.Disable(gl.TEXTURE_2D)
    gl.PopMatrix()
end

function glFont:textWidth(text)
    local width = 0
    if not text then return end

    for i = 1, #text do
        local co = string.byte(text, i) - 32

        if co < 1 or co > #(self.CharWidth) then
            -- Skip spaces and non-printables.
            width = width + self.CharWidth[1]
        else
            width = width + self.CharWidth[co]
        end
    end

    return width
end

function glFont:height()
    return self.Height
end
