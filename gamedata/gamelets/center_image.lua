-- Show an image at the center of the screen.
-- Switch to the menu gamelet when a key is pressed.

require "game"
require "sprite"

require "sdl"

gamelets.new(...)

function new(self, opts)
    local inst = gamelets.instance(self)
    inst.loaded_image = sprite:new(opts.image)
    return inst
end

function event(self, e)
    if e.type == SDL.SDL_KEYDOWN then
        gamelets.stack:pop()
        gamelets.stack:pop()
        gamelets.stack:push(gamelets.get("menu"):new{
            items = {7, 5, 9},
            default = 2
        })
        gamelets.stack:push(gamelets.get("top_text"):new{
            font = "test/bitstream-vera-sans-bold-24pt.png",
            text = "A menu is below"
        })
        return gamelets.GAMELET_REDRAW
    elseif e.type == SDL.SDL_QUIT then
        return gamelets.GAMELET_QUIT
    end
end

function update(self, delta)
    -- Nothing to update.
end

function draw(self)
    local screen_w, screen_h = game.getScreenSize()
    local x = (screen_w - self.loaded_image.w) / 2
    local y = (screen_h - self.loaded_image.h) / 2
    self.loaded_image:draw(x, y)
end
