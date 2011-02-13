local done = false
repeat
    gfx.clear()
    gfx.flip()
    local e = game.event(true)
    done = e.type == "exit"
until done
