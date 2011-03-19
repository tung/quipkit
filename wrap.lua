--[[ See copyright notice in doc/COPYRIGHT ]]

local old_package_path = package.path
package.path = './lib/?.lua;' .. package.path

require 'gl'
require 'glu'
require 'sdl'

local gl = gl
local glu = glu
local SDL = SDL

require 'gfx'

local gfx = gfx

package.path = old_package_path



local opts = ...



local function init()
    local width = opts.width
    local height = opts.height
    local fullscreen = opts.fullscreen
    local channels = opts.channels

    gfx.w = width
    gfx.h = height
    gfx.fullscreen = fullscreen

    if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
        error("Couldn't init SDL: " .. SDL.SDL_GetError())
    end

    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DOUBLEBUFFER, 1)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_RED_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_GREEN_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_BLUE_SIZE, 8)

    if not SDL.SDL_SetVideoMode(width, height, 32, SDL.SDL_OPENGL) then
        error("SDL_SetVideoMode: " .. SDL.SDL_GetError())
    end

    -- Set up OpenGL for 2D.
    gl.Viewport(0, 0, width, height)
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    glu.Ortho2D(0, width, height, 0)
    gl.MatrixMode(gl.MODELVIEW)
    gl.LoadIdentity()
    gl.Translate(0.375, 0.375, 0)

    -- Assumed clear colour used by gfx.clear.
    gl.ClearColor(0, 0, 0, 0)

    -- Texturing on by default.
    gl.Enable(gl.TEXTURE_2D)

    -- Accept byte-aligned textures.
    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)

    -- Calculate blending with source alpha.
    gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)
end


local function close()
    SDL.SDL_Quit()
end


return init, close
