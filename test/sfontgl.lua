require "game"
require "sfont"

function game.init()
    font = sfont.glFont:new("test/bitstream-vera-sans-bold-24pt.png")
end

function game.draw()
    game.clearScreen()
    font:write(0, 0, "SFont with OpenGL")
end

game.setScreenSize(320, 240)
game.run()
