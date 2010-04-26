require "game"
require "glpng"

local smile_texture

function game.init()
    SDL.WM.SetCaption("OpenGL with SDL", "OpenGL")
    smile_texture = glpng.PngBind("test/smile.png", glpng.NOMIPMAP, glpng.ALPHA, gl.CLAMP, gl.LINEAR, gl.LINEAR)
end

game.events[SDL.QUIT] = function ()
    return false
end

local milliseconds = 0
function game.update(delta)
    milliseconds = milliseconds + delta
    return milliseconds < 5000
end

function game.draw()
    gl.Viewport(80, 0, 480, 480)

    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Frustum(-1, 1, -1, 1, 1, 100)

    gl.MatrixMode(gl.MODELVIEW)
    gl.LoadIdentity()

    gl.ClearColor(0, 0, 0, 0)
    gl.Clear(gl.COLOR_BUFFER_BIT)

    gl.BindTexture(gl.TEXTURE_2D, smile_texture.id)
    gl.Begin(gl.QUADS)
        gl.TexCoord(0, 0)
        gl.Vertex(-1, -1, -2)
        gl.TexCoord(1, 0)
        gl.Vertex(1, -1, -2)
        gl.TexCoord(1, 1)
        gl.Vertex(1, 1, -2)
        gl.TexCoord(0, 1)
        gl.Vertex(-1, 1, -2)
    gl.End()

    gl.Flush()
end

game.run()
