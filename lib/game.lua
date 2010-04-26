-- Quipkit game engine framework.
-- Loads SDL and OpenGL, and drives the game loop.

require "sdl"
require "gl"

module(..., package.seeall)

events = {}

-- Called after SDL.Init().
init = function ()
end

-- Called before SDL.Exit().
exit = function ()
end

update = function (delta)
end

draw = function ()
end

-- Returns 'false' if any handler returns it.
-- This lets the user end the game loop.
local function handle_events()
    local event = SDL.PollEvent()
    while event do
        local handler = events[event.type]
        if type(handler) == "function" then
            if handler() == false then
                return false
            end
        end
        event = SDL.PollEvent()
    end
    return true
end

local function loop()
    local last, this
    last = SDL.GetTicks()
    if handle_events() then
        repeat
            this = SDL.GetTicks()
            if update(this - last) == false then
                -- Allow update() to end the game loop.
                return
            end
            last = this
            draw()
            SDL.GL.SwapBuffers()
        until not handle_events()
    end
end

run = function ()
    -- Is this a good way of going about things?
    assert(SDL.Init(SDL.INIT_EVERYTHING))
    init()

    -- TODO: Allow customisation of these.
    SDL.GL.SetAttribute(SDL.GL.DOUBLEBUFFER, 1)
    SDL.GL.SetAttribute(SDL.GL.RED_SIZE, 8)
    SDL.GL.SetAttribute(SDL.GL.GREEN_SIZE, 8)
    SDL.GL.SetAttribute(SDL.GL.BLUE_SIZE, 8)
    SDL.SetVideoMode(640, 480, 32, SDL.OPENGL)

    SDL.Assert(pcall(loop))

    exit()
    SDL.Quit()
end
