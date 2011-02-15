local smile = gfx.image:new('smile.png'):sub(8, 8, 16, 16)
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
    smile:draw(x, y)
    gfx.flip()
    game.sleep(10)
until done
