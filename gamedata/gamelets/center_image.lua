-- Show an image at the center of the screen.
-- Switch to the menu gamelet when a key is pressed.

require "game"
require "sprite"

require "sdl"

gamelet.new(...)

function init(opts)
    local self = gamelet.instance()
    self.loaded_image = sprite:new(opts.image)
    return self
end

function event(self, e)
    if e.type == SDL.SDL_KEYDOWN then
        gamelet.stack:pop()
        gamelet.stack:pop()
        gamelet.stack:push(gamelet.get("menu").init{
            items = {7, 5, 9},
            default = 2
        })
        gamelet.stack:push(gamelet.get("top_text").init{
            font = "test/bitstream-vera-sans-bold-24pt.png",
            text = "A menu is below"
        })
        return game.GAME_REDRAW
    elseif e.type == SDL.SDL_QUIT then
        return gamelet.GAMELET_QUIT
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
