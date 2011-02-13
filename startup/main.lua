local smile = gfx.image:new('smile.png')
local done = false
repeat
    gfx.clear()
    smile:draw(0, 0)
    gfx.flip()
    local e = game.event(true)
    done = e.type == "exit"
until done
