require "game"

require "gl"
require "png"
require "sdl"

local smile

function game.Init()
    SDL.SDL_WM_SetCaption("OpenGL with SDL", "OpenGL")

    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)

    smile = gl.GenTextures(1)[1]
    gl.BindTexture(gl.TEXTURE_2D, smile)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
    local smile_image = PNG.Open("test/smile.png")
    local gl_format = gl.RGBA
    local data_format = (smile_image.format == PNG.RGBA) and gl.RGBA or gl.RGB
    gl.TexImage2D(0, gl_format, smile_image.width, smile_image.height, 0, data_format, gl.UNSIGNED_BYTE, smile_image.data)
end

game.Events[SDL.SDL_QUIT] = function ()
    return false
end

local milliseconds = 0
function game.Update(delta)
    milliseconds = milliseconds + delta
    return milliseconds < 5000
end

function game.Draw()
    gl.Viewport(80, 0, 480, 480)

    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Frustum(-1, 1, -1, 1, 1, 100)

    gl.MatrixMode(gl.MODELVIEW)
    gl.LoadIdentity()

    gl.ClearColor(0, 0, 0, 0)
    gl.Clear(gl.COLOR_BUFFER_BIT)

    gl.Enable(gl.TEXTURE_2D)
    gl.TexEnv(gl.TEXTURE_ENV, gl.TEXTURE_ENV_MODE, gl.REPLACE)
    gl.BindTexture(gl.TEXTURE_2D, smile)
    gl.Begin(gl.QUADS)
        gl.TexCoord(0, 0)
        gl.Vertex(-1, 1, -2)
        gl.TexCoord(1, 0)
        gl.Vertex(1, 1, -2)
        gl.TexCoord(1, 1)
        gl.Vertex(1, -1, -2)
        gl.TexCoord(0, 1)
        gl.Vertex(-1, -1, -2)
    gl.End()
    gl.Flush()
    gl.Disable(gl.TEXTURE_2D)
end

game.Run()
