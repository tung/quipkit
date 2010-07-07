-- Quipkit game engine framework.
-- Loads SDL and OpenGL, and drives the game loop.

require "sdl"
require "gl"
require "png"

module(..., package.seeall)


events = {}

screen = {w = 640, h = 480, title = "Quipkit"}

-- Called after SDL.SDL_Init().
init = function ()
end

-- Called before SDL.SDL_Exit().
exit = function ()
end

update = function (delta)
end

draw = function ()
end

-- Returns 'false' if any handler returns it.
-- This lets the user end the game loop.
local _event
local function _handle_events()
    while SDL.SDL_PollEvent(_event) == 1 do
        local handler = events[_event.type]
        if type(handler) == "function" then
            if handler() == false then
                return false
            end
        end
    end
    return true
end

local function _loop()
    local last, this
    last = SDL.SDL_GetTicks()
    while _handle_events() do
        this = SDL.SDL_GetTicks()
        if update(this - last) == false then
            -- Allow Update() to end the game loop.
            return
        end
        last = this

        gl.MatrixMode(gl.MODELVIEW)
        gl.LoadIdentity()
        draw()
        gl.Flush()
        SDL.SDL_GL_SwapBuffers()
    end
end

function run()
    if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
        error("Failed to initialize SDL: " .. SDL.SDL_GetError(), 2)
    end

    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DOUBLEBUFFER, 1)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_RED_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_GREEN_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_BLUE_SIZE, 8)
    SDL.SDL_SetVideoMode(screen.w, screen.h, 32, SDL.SDL_OPENGL)

    SDL.SDL_WM_SetCaption(screen.title, "")

    -- Accept byte-aligned textures.
    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)

    -- Tell OpenGL where to render in our window.
    gl.Viewport(0, 0, screen.w, screen.h)

    -- Project the scenes-to-be orthographically, like in blue-prints.
    -- Also, switch bottom and top so (0, 0) is top-left and y extends down.
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Ortho(0, screen.w, screen.h, 0, -1, 1)

    init()

    _event = SDL.SDL_Event_new()

    local success, error_message
    success, error_message = pcall(_loop)
    if not success then
        SDL.SDL_Quit()
        error("Error in game loop: " .. error_message, 2)
    end

    exit()
    SDL.SDL_Quit()
end



--- Utility Functions

-- Clear the display to black.
function clearScreen()
    gl.ClearColor(0, 0, 0, 0)
    gl.Clear(gl.COLOR_BUFFER_BIT)
end
