--[[ See copyright notice in doc/COPYRIGHT ]]

require 'gl'
require 'sdl'

local gl = gl
local SDL = SDL



local opts = ...



local function init()
    local width = opts.width
    local height = opts.height
    local fullscreen = opts.fullscreen
    local channels = opts.channels

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

    -- Assumed clear colour used by gfx.clear.
    gl.ClearColor(0, 0, 0, 0)
end


local function close()
    SDL.SDL_Quit()
end


return init, close
