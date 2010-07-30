-- Quipkit sprite class.
-- A quick way of loading and drawing PNG images in OpenGL.

require "gl"
require "sdl"
require "sdlgl"

module(..., package.seeall)



function new(self, image_file, tile_w, tile_h)
    -- Load image into OpenGL.
    local image = SDL.IMG_Load(image_file)
    local tex = sdlgl.texture:new(image)
    SDL.SDL_FreeSurface(image)

    -- Tile support: allows drawing of fixed-size sub-images.
    local tw = tile_w and tile_w or tex.texW
    local th = tile_h and tile_h or tex.texH
    if tw < 1 or th < 1 then
        error("Cannot tile sprite by width or height < 1 (got w = " .. tw .. ", h = " .. th .. ")")
    end

    -- Standard Lua OOP setup.
    local s = {
        texture_id = tex.texId,
        w = tex.texW,
        h = tex.texH,
        tile_w = tw,
        tile_h = th,
        tile_x = 0,
        tile_y = 0
    }
    setmetatable(s, {__index = self})
    self.__index = self
    return s
end

function setTile(self, tile_x, tile_y)
    self.tile_x = tile_x
    self.tile_y = tile_y
end

function draw(self, x, y)
    -- Replace/draw over existing fragments (pixels).
    gl.TexEnv(gl.TEXTURE_ENV_MODE, gl.REPLACE)

    -- Move the sprite where it needs to be.
    gl.LoadIdentity()
    gl.Translate(x, y, 0)

    -- Draw the sprite.
    local tile_w, tile_h = self.tile_w, self.tile_h
    local tile_x, tile_y = self.tile_x, self.tile_y
    local offset_w = tile_w / self.w
    local offset_h = tile_h / self.h
    local offset_x = tile_x * offset_w
    local offset_y = tile_y * offset_h
    gl.Enable(gl.TEXTURE_2D)
        gl.BlendFunc(gl.SRC_ALPHA, gl.ONE)
        gl.BindTexture(gl.TEXTURE_2D, self.texture_id)
        gl.Begin(gl.QUADS)
            gl.TexCoord(offset_x, offset_y)
            gl.Vertex(0, 0)

            gl.TexCoord(offset_x + offset_w, offset_y)
            gl.Vertex(tile_w, 0)

            gl.TexCoord(offset_x + offset_w, offset_y + offset_h)
            gl.Vertex(tile_w, tile_h)

            gl.TexCoord(offset_x, offset_y + offset_h)
            gl.Vertex(0, tile_h)
        gl.End()
    gl.Disable(gl.TEXTURE_2D)
end
