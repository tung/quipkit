require "game"

local glob
-- tile_advance is frames per second
local tile, num_tiles = 0, 4
local tile_tick, tile_advance = 0, 0.25
-- y_speed is pixels per second
local x, y, y_speed = 320, 0, 128

function game.Init()
    math.randomseed(os.time())
    glob = game.Sprite:new("test/glob.png", 32, 32)
end

game.Events[SDL.SDL_QUIT] = function ()
    return false
end

function game.Update(ms_delta)
    local s_delta = ms_delta / 1000

    -- Move Glob down the screen.
    y = y + y_speed * s_delta
    if y >= game.Screen.h then
        x = math.random(game.Screen.w - glob.w - 1)
        y = -glob.h
    end

    -- Animate Glob.
    tile_tick = tile_tick + s_delta
    while tile_tick >= tile_advance do
        tile = (tile + 1) % num_tiles
        glob:SetTile(tile, 1)
        tile_tick = tile_tick - tile_advance
    end
end

function game.Draw()
    game.ClearScreen()
    glob:Draw(x, y)
end

game.Screen.w, game.Screen.h = 640, 480
game.Screen.title = "Animation Demo"

game.Run()
