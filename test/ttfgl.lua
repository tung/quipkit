require "game"
require "sprite"

local msg

function game.init()
    local font = SDL.TTF_OpenFont("test/Vera.ttf", 24)
    msg = sprite:newTextBlended(font, "GREEeennn...!", {0, 255, 0})
end

function game.draw()
    game.clearScreen()
    msg:draw(0, 0)
end

game.setScreenSize(320, 240)
game.setScreenTitle("SDL_ttf in OpenGL")
game.run()
