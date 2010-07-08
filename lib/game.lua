-- Quipkit game engine framework.
-- Loads SDL and OpenGL, and drives the game loop.

require "gl"
require "png"
require "sdl"

module(..., package.seeall)



--- game.lua public hooks. Assign to these to change how your game runs.

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



--- Game loop and execution.

-- Main game loop. Called by game.run().
local function _loop()
    local current_time, last_time
    current_time = SDL.SDL_GetTicks()
    repeat
        last_time = current_time

        gl.MatrixMode(gl.MODELVIEW)
        gl.LoadIdentity()
        draw()
        gl.Flush()
        SDL.SDL_GL_SwapBuffers()

        current_time = SDL.SDL_GetTicks()
    until update(current_time - last_time) == false
end


-- Cached SDL event struct. Allocated once in game.run(),
-- then returned by game.waitEvent() and game.pollEvent().
local _event

-- Set up game environment, launch the game loop and clean up afterwards.
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


-- Check if there are any events in the queue, and return the next one if there is.
function pollEvent()
    if SDL.SDL_PollEvent(_event) == 1 then
        return _event
    end
    return nil
end


-- Wait for an event to occur and return it.
function waitEvent()
    if SDL.SDL_WaitEvent(_event) == 0 then
        error("SDL_WaitEvent failed: " .. SDL.SDL_GetError())
    end
    return _event
end
