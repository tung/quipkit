-- Quipkit game engine framework.
-- Loads SDL and OpenGL, and drives the game loop.

require "gl"
require "sdl"

module(..., package.seeall)



--- game.lua public hooks. Assign to these to change how your game runs.

-- Called after SDL.SDL_Init().
init = function ()
end

-- Called before SDL.SDL_Exit().
exit = function ()
end

-- Valid return values for event()/update().
GAME_QUIT = 1       -- Stop the game loop.
GAME_REDRAW = 2     -- Call the draw() function.

-- User-defined event hook.
event = function (e)
    if e.type == SDL.SDL_VIDEOEXPOSE or e.type == SDL.SDL_ACTIVEEVENT then
        return GAME_REDRAW
    elseif e.type == SDL.SDL_QUIT then
        return GAME_QUIT
    end
end

-- User-defined game logic update hook. Should usually return GAME_REDRAW.
update = function (delta)
    SDL.SDL_Delay(10)
end

-- User-defined rendering hook.
draw = function ()
end



--- Game configuration.

local config = {
    screen = {
        w = 640,
        h = 480,
        title = "Quipkit"
    },
}



--- Game loop and execution.

-- Cached SDL event struct. Allocated once in game.run(),
-- then used in the game loop.
local _event

-- Main game loop. Called by game.run()
local function _loop()
    local e_result = GAME_REDRAW
    local u_result = GAME_REDRAW
    local current_time
    local last_time = SDL.SDL_GetTicks()
    while true do
        -- Draw.
        if e_result == GAME_REDRAW or u_result == GAME_REDRAW then
            gl.MatrixMode(gl.MODELVIEW)
            gl.LoadIdentity()
            draw()
            gl.Flush()
            SDL.SDL_GL_SwapBuffers()
        end

        -- Handle events.
        while SDL.SDL_PollEvent(_event) == 1 do
            e_result = event(_event)
        end
        if e_result == GAME_QUIT then return end

        -- Update.
        current_time = SDL.SDL_GetTicks()
        u_result = update(current_time - last_time)
        last_time = current_time
        if u_result == GAME_QUIT then return end
    end
end


-- Set up game environment, launch the game loop and clean up afterwards.
function run()
    if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
        error("Failed to initialize SDL: " .. SDL.SDL_GetError(), 2)
    end

    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DOUBLEBUFFER, 1)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_RED_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_GREEN_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_BLUE_SIZE, 8)
    SDL.SDL_SetVideoMode(config.screen.w, config.screen.h, 32, SDL.SDL_OPENGL)

    SDL.SDL_WM_SetCaption(config.screen.title, "")

    -- Accept byte-aligned textures.
    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)

    -- Account for transparency when drawing fragments.
    gl.Enable(gl.BLEND)
    gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)

    -- Tell OpenGL where to render in our window.
    gl.Viewport(0, 0, config.screen.w, config.screen.h)

    -- Project the scenes-to-be orthographically, like in blue-prints.
    -- Also, switch bottom and top so (0, 0) is top-left and y extends down.
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Ortho(0, config.screen.w, config.screen.h, 0, -1, 1)

    init()

    _event = SDL.SDL_Event_local()

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



--- Configuration Functions

-- Get the width and height of the game window.
function getScreenSize()
    return config.screen.w, config.screen.h
end


-- Get the title of the game window.
function getScreenTitle()
    return config.screen.title
end


-- Set the width and height of the game window.
function setScreenSize(w, h)
    local old_w = config.screen.w
    local old_h = config.screen.h
    config.screen.w = w
    config.screen.h = h
    return old_w, old_h
end


-- Set the title of the game window.
function setScreenTitle(title)
    local old_title = config.screen.title
    config.screen.title = title
    return old_title
end
