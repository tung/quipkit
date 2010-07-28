require "game"
require "sdl"
require "sdlgl"

local smile

function game.init()
	local smile_surface = SDL.IMG_Load("test/smile.png")
	smile = sdlgl.Texture:new(smile_surface)
	SDL.SDL_FreeSurface(smile_surface)
end

function game.draw()
	game.clearScreen()

	local x, y, w, h
	x = math.floor((game.screen.w - smile.w) / 2)
	y = math.floor((game.screen.h - smile.h) / 2)
	w = smile.texW
	h = smile.texH

	gl.PushMatrix()
		gl.LoadIdentity()
		gl.Translate(x, y, 0)
		gl.Enable(gl.TEXTURE_2D)
			gl.BlendFunc(gl.SRC_ALPHA, gl.ONE)
			gl.BindTexture(gl.TEXTURE_2D, smile.texId)
			gl.Begin(gl.QUADS)
				gl.TexCoord(0, 0); gl.Vertex(0, 0)
				gl.TexCoord(1, 0); gl.Vertex(w, 0)
				gl.TexCoord(1, 1); gl.Vertex(w, h)
				gl.TexCoord(0, 1); gl.Vertex(0, h)
			gl.End()
		gl.Disable(gl.TEXTURE_2D)
	gl.PopMatrix()
end

game.screen.w, game.screen.h = 320, 240
game.run()
