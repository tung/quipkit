require "game"
require "sdlgl"
require "sprite"

local msg

function game.init()
    local font = SDL.TTF_OpenFont("test/Vera.ttf", 24)
    msg = sdlgl.texture:newTextBlended(font, "SDL_ttf in OpenGL", {255, 255, 255})
end

function game.draw()
    game.clearScreen()
    gl.PushMatrix()
        gl.LoadIdentity()
        gl.Enable(gl.TEXTURE_2D)
            gl.BindTexture(gl.TEXTURE_2D, msg.texId)
            gl.Begin(gl.QUADS)
                gl.TexCoord(0, 0); gl.Vertex(0, 0)
                gl.TexCoord(1, 0); gl.Vertex(msg.w, 0)
                gl.TexCoord(1, 1); gl.Vertex(msg.w, msg.h)
                gl.TexCoord(0, 1); gl.Vertex(0, msg.h)
            gl.End()
        gl.Disable(gl.TEXTURE_2D)
    gl.PopMatrix()
end

game.setScreenSize(320, 240)
game.setScreenTitle("TTF in OpenGL")
game.run()
