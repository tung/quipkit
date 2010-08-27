-- Example main code to start using gamelets.

-- How to use gamelets:
--
-- Write your gamelets in the gamedata/gamelets/ directory, or point
-- gamelet.location to where your gamelets are.
--
-- Set up the gamelet stack when your game inits by pushing gamelet
-- instances onto it.
--
-- Hook up the gamelet stack's event, update and draw hooks to the
-- matching hooks in your game.
--
-- While the game is running, gamelets may change the gamelet stack
-- by removing gamelet instances and adding new ones.

require "game"
require "gamelet"

function game.init()
    table.insert(gamelet.stack, gamelet("center_image"):new{
        image = "test/smile.png"
    })
    table.insert(gamelet.stack, gamelet("top_text"):new{
        font = "test/bitstream-vera-sans-bold-24pt.png",
        text = "Press any key"
    })
end

function game.event(e)
    return gamelet.stack:event(e)
end

function game.update(delta)
    return gamelet.stack:update(delta)
end

function game.draw()
    game.clearScreen()
    return gamelet.stack:draw()
end

game.setScreenTitle("Gamelet Test")
game.run()
