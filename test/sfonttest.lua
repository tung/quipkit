require "game"
require "gl"
require "sdl"
require "sfont"

local font_file = "test/bitstream-vera-sans-bold-24pt.png"
local msg = "SFont works in Quipkit!"
local msg_texture, msg_w, msg_h

function game.init()
    local font_image = SDL.IMG_Load(font_file)
    if not font_image then
        error("IMG_Load failed: " .. SDL.IMG_GetError())
    end
    local font = sfont:new(font_image)
    if not font then
        error("sfont:new failed: Could not load " .. font_file)
    end

    -- Draw message onto an intermediate surface.
    -- Round up width and height to nearest power of 2 for OpenGL.
    msg_w = math.pow(2, math.ceil(math.log(font:textWidth(msg)) / math.log(2)))
    msg_h = math.pow(2, math.ceil(math.log(font:height()) / math.log(2)))
    local rmask, gmask, bmask, amask
    if SDL.SDL_BYTEORDER == SDL.SDL_LIL_ENDIAN then
        rmask = 0x000000ff
        gmask = 0x0000ff00
        bmask = 0x00ff0000
        amask = 0xff000000
    else
        rmask = 0xff000000
        gmask = 0x00ff0000
        bmask = 0x0000ff00
        amask = 0x000000ff
    end
    local msg_image = SDL.SDL_CreateRGBSurface(SDL.SDL_SWSURFACE, msg_w, msg_h, 32, rmask, gmask, bmask, amask)
    if not msg_image then
        error("SDL_CreateRGBSurface failed: " .. SDL.SDL_GetError())
    end
    font:write(msg_image, 0, 0, msg)

    -- Load rendered text into an OpenGL texture.
    -- Screw error-checking.
    msg_texture = gl.GenTextures(1)[1]
    gl.BindTexture(gl.TEXTURE_2D, msg_texture)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
    gl.TexImage2D(0, gl.RGBA, msg_w, msg_h, 0, gl.RGBA, gl.UNSIGNED_BYTE, msg_image.pixels)

    SDL.SDL_FreeSurface(msg_image)
    SDL.SDL_FreeSurface(font_image)
end

function game.exit()
    gl.DeleteTextures({msg_texture})
end

function game.draw()
    game.clearScreen()

    gl.TexEnv(gl.TEXTURE_ENV, gl.TEXTURE_ENV_MODE, gl.REPLACE)

    gl.LoadIdentity()

    gl.Enable(gl.TEXTURE_2D)
        gl.BlendFunc(gl.SRC_ALPHA, gl.ONE)
        gl.BindTexture(gl.TEXTURE_2D, msg_texture)
        gl.Begin(gl.QUADS)
            gl.TexCoord(0, 0); gl.Vertex(    0,     0)
            gl.TexCoord(1, 0); gl.Vertex(msg_w,     0)
            gl.TexCoord(1, 1); gl.Vertex(msg_w, msg_h)
            gl.TexCoord(0, 1); gl.Vertex(    0, msg_h)
        gl.End()
    gl.Disable(gl.TEXTURE_2D)
end

game.screen.w, game.screen.h = 320, 240
game.run()
