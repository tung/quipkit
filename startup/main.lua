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


local smile = gfx.image:new('smile.png')
local smile_tiles = smile:tiles(16, 16)
local smile_tl = smile_tiles[1][1]
local smile_tr = smile_tiles[1][2]
local smile_bl = smile_tiles[2][1]
local smile_br = smile_tiles[2][2]

local x = math.floor((gfx.w - smile.w) / 2)
local y = math.floor((gfx.h - smile.h) / 2)
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
        x = x < 0 and 0 or (x + smile.w >= gfx.w and gfx.w - smile.w - 1 or x)
    end
    if dy ~= 0 then
        y = y + dy
        y = y < 0 and 0 or (y + smile.h >= gfx.h and gfx.h - smile.h - 1 or y)
    end

    ibuf_map:draw()
    smile_br:draw(x, y)
    smile_bl:draw(x + smile_br.w, y)
    smile_tr:draw(x, y + smile_br.h)
    smile_tl:draw(x + smile_br.w, y + smile_br.h)
    gfx.flip()
until done
