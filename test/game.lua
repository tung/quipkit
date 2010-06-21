require "game"

game.Events[SDL.SDL_QUIT] = function ()
    return false
end

local milliseconds = 0
function game.Update(delta)
    milliseconds = milliseconds + delta
    return milliseconds < 5000
end

function game.Draw()
    game.ClearScreen()

    gl.Begin(gl.TRIANGLES)
        gl.Color(1, 0, 0)
        gl.Vertex(320, 120)
        gl.Color(0, 1, 0)
        gl.Vertex(195, 360)
        gl.Color(0, 0, 1)
        gl.Vertex(445, 360)
    gl.End()
end

game.Screen.w, game.Screen.h = 640, 480
game.Screen.title = "Colored Triangle"

game.Run()
