-- Example main code to start using gamelets.

-- How to use gamelets:
--
-- First, write your gamelets in the gamelets/ directory.
--
-- To use gamelets, make instances of them, and put them on
-- the gamelet stack.
--
-- The gamelet instance on top of the stack will have its event(),
-- update() and draw() functions called once everything is running.
--
-- Gamelet instances can pop themselves and push other gamelet
-- instances over the course of a game.
--
-- Finally, setting up gamelets.init() and calling gamelets.run()
-- as below kicks off the whole thing.

require "gamelets"

print([[
Press a key to go from the smile to the globs.
Press UP and DOWN at the globs to move, RETURN to go back to the smile.
Press ESCAPE at the globs to exit.]])

function gamelets.init()
    gamelets.stack:push(gamelets("center_image"):new{
        image = "test/smile.png"
    })
    gamelets.stack:push(gamelets("top_text"):new{
        font = "test/bitstream-vera-sans-bold-24pt.png",
        text = "A title in the background."
    })
end

gamelets.run()
