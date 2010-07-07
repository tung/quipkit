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

function update(self, delta)
    local e = game.waitEvent()
    --if e.type == game.EVENT_KEYDOWN then
    if e.type == SDL.SDL_KEYDOWN then
        gamelet.stack:pop()
        gamelet.stack:push(gamelet.load("menu").init{
            items = {7, 5, 9},
            default = 2
        })
        --return game.UPDATE_REDRAW
    end
    --return game.UPDATE_NONE
    return true
end

function draw(self)
    game.clearScreen()

    local x = (game.screen.w - self.loaded_image.w) / 2
    local y = (game.screen.h - self.loaded_image.h) / 2
    self.loaded_image:draw(x, y)
end
