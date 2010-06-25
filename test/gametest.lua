require "game"

local smile
local x, y, dx, dy = 0, 0, 1, 1

function game.init()
    smile = game.sprite:new("test/smile.png")
end

game.events[SDL.SDL_QUIT] = function ()
    -- Returning false in any event ends the game loop.
    return false
end

function game.update(delta)
    x = x + dx
    y = y + dy
    if x < 0 then
        x = 0
        dx = -dx
    elseif x + smile.w >= game.screen.w then
        x = game.screen.w - 1 - smile.w
        dx = -dx
    end
    if y < 0 then
        y = 0
        dy = -dy
    elseif y + smile.h >= game.screen.h then
        y = game.screen.h - 1 - smile.h
        dy = -dy
    end
    return true
end

function game.draw()
    game.clearScreen()
    smile:draw(x, y)
end

-- Set screen properties.
game.screen.w, game.screen.h = 640, 480
game.screen.title = "Quipkit Engine Demo"

-- Launch our game.
game.run()
