require "game"
require "gl"
require "sdl"

local smile

function game.init()
    SDL.SDL_WM_SetCaption("OpenGL with SDL", "OpenGL")

    local smile_image_orig = SDL.IMG_Load("test/smile.png")
    local smile_image = SDL.SDL_CreateRGBSurface(SDL.SDL_SWSURFACE, smile_image_orig.w, smile_image_orig.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000)
    if not smile_image then
        error("SDL_CreateRGBSurface failed: " .. SDL.SDL_GetError())
    end
    if SDL.SDL_BlitSurface(smile_image_orig, nil, smile_image, nil) ~= 0 then
        error("SDL_BlitSurface failed: " .. SDL.SDL_GetError())
    end
    SDL.SDL_FreeSurface(smile_image_orig)

    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)

    smile = gl.GenTextures(1)[1]
    gl.BindTexture(gl.TEXTURE_2D, smile)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
    gl.TexImage2D(0, gl.RGBA, smile_image.w, smile_image.h, 0, gl.RGBA, gl.UNSIGNED_BYTE, smile_image.pixels)
end

function game.exit()
    gl.DeleteTextures({smile})
end

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

game.run()
