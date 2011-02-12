local done = false
repeat
    game.sleep(10)
    local e = game.event(true)
    done = e.type == "exit"
until done
