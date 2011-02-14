local smile = gfx.image:new('smile.png')
local x, y = 0, 0
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
    if dx ~= 0 then x = x + dx end
    if dy ~= 0 then y = y + dy end

    gfx.clear()
    smile:draw(x, y)
    gfx.flip()
    game.sleep(10)
until done
