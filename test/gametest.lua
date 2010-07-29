require "game"
require "sdl"
require "sprite"

local smile
local x, y, dx, dy = 0, 0, 1, 1

function game.init()
    smile = sprite.sprite:new("test/smile.png")
end

function game.event(e)
    if e.type == SDL.SDL_QUIT then
        return game.GAME_QUIT
    end
end

function game.update(delta)
    local screen_w, screen_h = game.getScreenSize()
    x = x + dx
    y = y + dy
    if x < 0 then
        x = 0
        dx = -dx
    elseif x + smile.w >= screen_w then
        x = screen_w - 1 - smile.w
        dx = -dx
    end
    if y < 0 then
        y = 0
        dy = -dy
    elseif y + smile.h >= screen_h then
        y = screen_h - 1 - smile.h
        dy = -dy
    end

    return game.GAME_REDRAW
end

function game.draw()
    game.clearScreen()
    smile:draw(x, y)
end

-- Set screen properties.
game.setScreenSize(640, 480)
game.setScreenTitle("Quipkit Engine Demo")

-- Launch our game.
game.run()
