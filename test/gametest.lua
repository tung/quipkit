require "game"
require "sdl"

local smile
local x, y, dx, dy = 0, 0, 1, 1

function game.Init()
    SDL.SDL_WM_SetCaption("Quipkit", "Demo")
    smile = game.Sprite:new("test/smile.png")
end

game.Events[SDL.SDL_QUIT] = function ()
    -- Returning false in any event ends the game loop.
    return false
end

function game.Update(delta)
    x = x + dx
    y = y + dy
    if x < 0 then
        x = 0
        dx = -dx
    elseif x + smile.w >= game.Screen.w then
        x = game.Screen.w - 1 - smile.w
        dx = -dx
    end
    if y < 0 then
        y = 0
        dy = -dy
    elseif y + smile.h >= game.Screen.h then
        y = game.Screen.h - 1 - smile.h
        dy = -dy
    end
    return true
end

function game.Draw()
    game.ClearScreen()
    smile:Draw(x, y)
end

-- Set the screen resolution.
game.Screen = {w = 640, h = 480}

-- Launch our game.
game.Run()
