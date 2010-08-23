require "game"
require "sprite"

local msg_so, msg_sh, msg_bl

function game.init()
    local font = SDL.TTF_OpenFont("test/Vera.ttf", 24)
    msg_so = sprite:newTextSolid(font, "newTextSolid", {255, 0, 0})
    msg_sh = sprite:newTextShaded(font, "newTextShaded", {0, 255, 0}, {64, 64, 64})
    msg_bl = sprite:newTextBlended(font, "newTextBlended", {0, 0, 255})
end

function game.draw()
    game.clearScreen()
    msg_so:draw(0, 0)
    msg_sh:draw(32, msg_so.h)
    msg_bl:draw(64, msg_so.h + msg_sh.h)
end

game.setScreenSize(320, 240)
game.setScreenTitle("SDL_ttf in OpenGL")
game.run()
