--[[ See copyright notice in doc/COPYRIGHT ]]

local old_package_path = package.path
package.path = './lib/?.lua;' .. package.path

require 'gl'
require 'sdl'

local gl = gl
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

    -- Set up OpenGL.
    gl.Viewport(0, 0, width, height)
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Ortho(0, width, height, 0, -1, 1)

    -- Assumed clear colour used by gfx.clear.
    gl.ClearColor(0, 0, 0, 0)

    -- Texturing on by default.
    gl.Enable(gl.TEXTURE_2D)

    -- Accept byte-aligned textures.
    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)
end


local function close()
    SDL.SDL_Quit()
end


return init, close
