--[[ See copyright notice in doc/COPYRIGHT. ]]

require 'gl'
require 'sdl'
require 'sdlgl'

local gl = gl
local SDL = SDL
local sdlgl = sdlgl

local setmetatable = setmetatable

module(...)



-- show everything drawn to the screen
function flip()
    SDL.SDL_GL_SwapBuffers()
end


-- clear the screen
function clear()
    -- Assume the clear colour is set correctly.
    gl.Clear(gl.COLOR_BUFFER_BIT)
end



--[[ gfx.image object ]]

image = {}
image.__index = image

-- load an image from the given path
function image:new(path)
    local image = SDL.IMG_Load(path)
    if not image then
        error("IMG_Load: " .. SDL.IMG_GetError())
    end
    local texture = sdlgl.texture:new(image)
    SDL.SDL_FreeSurface(image)

    local o = {
        w = texture.texW,
        h = texture.texH,
        path = path,
        _texture = texture
    }
    setmetatable(o, self)
    return o
end


-- draw an image on the screen
function image:draw(x, y)
    gl.BindTexture(gl.TEXTURE_2D, self._texture.texId)
    gl.Begin(gl.QUADS)
        gl.TexCoord(0, 0)
        gl.Vertex(x, y)

        gl.TexCoord(1, 0)
        gl.Vertex(x + self.w, y)

        gl.TexCoord(1, 1)
        gl.Vertex(x + self.w, y + self.h)

        gl.TexCoord(0, 1)
        gl.Vertex(x, y + self.h)
    gl.End()
end



return _M
