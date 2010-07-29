require "game"
require "gl"
require "sdl"

function game.event(e)
    if e.type == SDL.SDL_QUIT then
        return game.GAME_QUIT
    end
end

local milliseconds = 0
function game.update(delta)
    milliseconds = milliseconds + delta
    if milliseconds >= 5000 then
        return game.GAME_QUIT
    end
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

game.setScreenSize(640, 480)
game.setScreenTitle("Colored Triangle")

game.run()
