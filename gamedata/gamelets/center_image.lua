-- Show an image at the center of the screen.
-- Switch to the menu gamelet when a key is pressed.

require "game"
require "sdl"
require "sprite"

module(..., package.seeall)

MT = { __index = getfenv() }

function new(g, o)
    local inst = o or {}
    inst.loaded_image = sprite:new(inst.image)
    setmetatable(inst, MT)
    return inst
end

function event(self, e)
    if e.type == SDL.SDL_KEYDOWN then
        table.remove(gamelet.stack)
        table.remove(gamelet.stack)
        table.insert(gamelet.stack, gamelet("menu"):new{
            items = {7, 5, 9},
            default = 2
        })
        table.insert(gamelet.stack, gamelet("top_text"):new{
            font = "test/bitstream-vera-sans-bold-24pt.png",
            text = "A menu is below"
        })
        return game.GAME_REDRAW
    elseif e.type == SDL.SDL_QUIT then
        return game.GAME_QUIT
    end
end

function draw(self)
    local screen_w, screen_h = game.getScreenSize()
    local x = (screen_w - self.loaded_image.w) / 2
    local y = (screen_h - self.loaded_image.h) / 2
    self.loaded_image:draw(x, y)
end
