require "game"
require "sprite"

local glob
-- tile_advance is frames per second
local tile, num_tiles = 0, 4
local tile_tick, tile_advance = 0, 0.25
-- y_speed is pixels per second
local x, y, y_speed = 320, 0, 128

function game.init()
    math.randomseed(os.time())
    glob = sprite.sprite:new("test/glob.png", 32, 32)
end

function game.event(e)
    if e.type == SDL.SDL_QUIT then
        return game.GAME_QUIT
    end
end

function game.update(ms_delta)
    -- Move Glob down the screen.
    local s_delta = ms_delta / 1000
    y = y + y_speed * s_delta
    if y >= game.screen.h then
        x = math.random(game.screen.w - glob.w - 1)
        y = -glob.h
    end

    -- Animate Glob.
    tile_tick = tile_tick + s_delta
    while tile_tick >= tile_advance do
        tile = (tile + 1) % num_tiles
        glob:setTile(tile, 1)
        tile_tick = tile_tick - tile_advance
    end

    return game.GAME_REDRAW
end

function game.draw()
    game.clearScreen()
    glob:draw(x, y)
end

game.screen.w, game.screen.h = 640, 480
game.screen.title = "Animation Demo"

game.run()
