-- Example main code to start using gamelets.

-- How to use gamelets:
--
-- First, write your gamelets in the gamelets/ directory.
--
-- To use gamelets, make instances of them, and put them on
-- the gamelet stack.
--
-- The gamelet instance on top of the stack will have its update()
-- and draw() functions called once everything is running.
--
-- Gamelet instances can pop themselves and push other gamelet
-- instances over the course of a game.
--
-- Finally, setting up gamelet.init() and calling gamelet.run()
-- as below kicks off the whole thing.

require "gamelet"

print([[
Press a key to go from the smile to the globs.
Press UP and DOWN at the globs to move, RETURN to go back to the smile.
Press ESCAPE at the globs to exit.

NOTE: The smile won't show at first. Press a key to go to the globs.]])

function gamelet.init()
    gamelet.stack:push(gamelet.load("center_image").init{
        image = "test/smile.png"
    })
end

gamelet.run()
