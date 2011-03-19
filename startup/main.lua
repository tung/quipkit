local function BufferTileMap()
    local map = {
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3},
        {3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3},
        {3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3},
        {3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3},
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    }
    local map_w = #(map[1])
    local map_h = #map

    local ibuf_map = gfx.ibuf:new(gfx.image:new('tiles.png'))
    local TILE_SIZE = 16
    ibuf_map:settile(TILE_SIZE)
    for map_y = 1, math.ceil(gfx.h / TILE_SIZE) do
        for map_x = 1, math.ceil(gfx.w / TILE_SIZE) do
            local tile_x = (map_x - 1) % map_w + 1
            local tile_y = (map_y - 1) % map_h + 1
            ibuf_map:addtile(map[tile_y][tile_x], 1, (map_x - 1) * TILE_SIZE, (map_y - 1) * TILE_SIZE)
        end
    end
    ibuf_map:pack()

    return ibuf_map
end


local player = gfx.image:new('glob.png'):tiles(32, 32)[1][1]

local x = math.floor((gfx.w - player.w) / 2)
local y = math.floor((gfx.h - player.h) / 2)
local dx, dy, speed = 0, 0, 4

local ibuf_map = BufferTileMap()


local done = false
repeat
    local e = game.event()
    if e then
        if e.type == 'keydown' then
            if e.key == 'up'    then dy = dy - speed end
            if e.key == 'down'  then dy = dy + speed end
            if e.key == 'left'  then dx = dx - speed end
            if e.key == 'right' then dx = dx + speed end
        elseif e.type == 'keyup' then
            if e.key == 'up'    then dy = dy + speed end
            if e.key == 'down'  then dy = dy - speed end
            if e.key == 'left'  then dx = dx + speed end
            if e.key == 'right' then dx = dx - speed end
        elseif e.type == 'exit' then
            done = true
        end
    end
    if dx ~= 0 then
        x = x + dx
        x = x < 0 and 0 or (x + player.w >= gfx.w and gfx.w - player.w - 1 or x)
    end
    if dy ~= 0 then
        y = y + dy
        y = y < 0 and 0 or (y + player.h >= gfx.h and gfx.h - player.h - 1 or y)
    end

    ibuf_map:draw()
    player:draw(x, y)
    gfx.flip()
until done
