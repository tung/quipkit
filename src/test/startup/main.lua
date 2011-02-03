local done = false
repeat
    gfx.sysfont:draw("Hello, World!", 0, 0)
    gfx.flip()
    local ev = game.event(true)
    done = ev.type == 'exit' or ev.type == 'keydown'
until done
