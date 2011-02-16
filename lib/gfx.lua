--[[ See copyright notice in doc/COPYRIGHT. ]]

require 'gl'
require 'sdl'
require 'sdlgl'

local gl = gl
local SDL = SDL
local sdlgl = sdlgl

local math = math
local table = table

local error = error
local getmetatable = getmetatable
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

    local tex_w = texture.texW
    local tex_h = texture.texH

    local o = {
        w = tex_w,
        h = tex_h,
        path = path,
        _texture = texture,
        _texture_x1 = 0,
        _texture_y1 = 0,
        _texture_x2 = 1,
        _texture_y2 = 1
    }
    setmetatable(o, self)
    return o
end


-- draw an image on the screen
function image:draw(x, y)
    local tex_x1 = self._texture_x1
    local tex_x2 = self._texture_x2
    local tex_y1 = self._texture_y1
    local tex_y2 = self._texture_y2

    gl.BindTexture(gl.TEXTURE_2D, self._texture.texId)
    gl.Begin(gl.QUADS)
        gl.TexCoord(tex_x1, tex_y1)
        gl.Vertex(x, y)

        gl.TexCoord(tex_x2, tex_y1)
        gl.Vertex(x + self.w, y)

        gl.TexCoord(tex_x2, tex_y2)
        gl.Vertex(x + self.w, y + self.h)

        gl.TexCoord(tex_x1, tex_y2)
        gl.Vertex(x, y + self.h)
    gl.End()
end


-- create a new sub-image gfx.image object from an image
function image:sub(x, y, w, h)
    if self._texture_x1 + x < 0 then
        error("left edge of sub-image must be >= 0 (x = " .. x .. ")")
    end
    if self._texture_x1 + x + w > self._texture.texW then
        error("right edge of sub-image must be <= texture width (" ..
                self._texture.texW .. ") (x = " .. x .. "; w = " .. w .. ")")
    end
    if self._texture_y1 + y < 0 then
        error("top edge of sub-image must be >= 0 (y = " .. y .. ")")
    end
    if self._texture_y1 + y + h > self._texture.texH then
        error("bottom edge of sub-image must be <= texture height (" ..
                self._texture.texH .. ") (y = " .. y .. "; h = " .. h .. ")")
    end

    local o = {
        w = w,
        h = h,
        path = self.path,
        _texture = self._texture,
        _texture_x1 = self._texture_x1 + (x / self._texture.texW),
        _texture_x2 = self._texture_x1 + ((x + w) / self._texture.texW),
        _texture_y1 = self._texture_y1 + (y / self._texture.texH),
        _texture_y2 = self._texture_y1 + ((y + h) / self._texture.texH)
    }
    setmetatable(o, getmetatable(self))
    return o
end


-- create a list of rows of sub-images, each with the given width and height
function image:tiles(tilew, tileh)
    if tilew <= 0 then
        error("tilew (" .. tilew .. ") must be > 0")
    end
    if tileh <= 0 then
        error("tileh (" .. tileh .. ") must be > 0")
    end

    local rows = {}
    for r = 1, math.floor(self.h / tileh) do
        local row = {}
        for c = 1, math.floor(self.w / tilew) do
            table.insert(row, self:sub((c - 1) * tilew, (r - 1) * tileh, tilew, tileh))
        end
        table.insert(rows, row)
    end
    return rows
end



return _M
