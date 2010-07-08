require "game"
require "gl"
require "sdl"

local milliseconds = 0
function game.update(delta)
    local e = game.pollEvent()
    if e and e.type == SDL.SDL_QUIT then
        return false
    end

    milliseconds = milliseconds + delta
    return milliseconds < 5000
end

function game.draw()
    game.clearScreen()

    gl.Begin(gl.TRIANGLES)
        gl.Color(1, 0, 0)
        gl.Vertex(320, 120)
        gl.Color(0, 1, 0)
        gl.Vertex(195, 360)
        gl.Color(0, 0, 1)
        gl.Vertex(445, 360)
    gl.End()
end

game.screen.w, game.screen.h = 640, 480
game.screen.title = "Colored Triangle"

game.run()
