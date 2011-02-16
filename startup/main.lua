local smile = gfx.image:new('smile.png')
local smile_tiles = smile:tiles(16, 16)
local smile_tl = smile_tiles[1][1]
local smile_tr = smile_tiles[1][2]
local smile_bl = smile_tiles[2][1]
local smile_br = smile_tiles[2][2]

local x = math.floor((gfx.w - smile.w) / 2)
local y = math.floor((gfx.h - smile.h) / 2)
local dx, dy, speed = 0, 0, 4

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

    gfx.clear()
    smile_br:draw(x, y)
    smile_bl:draw(x + smile_br.w, y)
    smile_tr:draw(x, y + smile_br.h)
    smile_tl:draw(x + smile_br.w, y + smile_br.h)
    gfx.flip()
    game.sleep(10)
until done
