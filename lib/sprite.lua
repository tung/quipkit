-- Quipkit sprite class.
-- A quick way of loading and drawing PNG images in OpenGL.

require "gl"
require "png"

module(..., package.seeall)



function new(self, image_file, tile_w, tile_h)
    -- Load image into OpenGL texture.
    local image = PNG.Open(image_file)
    local texture_id = gl.GenTextures(1)[1]
    gl.BindTexture(gl.TEXTURE_2D, texture_id)
    -- Tell OpenGL how to stretch/shrink our texture while we're at it.
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
    local gl_format = gl.RGBA
    local data_format
    local use_alpha
    if image.format == PNG.RGBA then
        data_format = gl.RGBA
        use_alpha = true
    else
        data_format = gl.RGB
        use_alpha = false
    end
    gl.TexImage2D(0, gl_format, image.width, image.height, 0, data_format, gl.UNSIGNED_BYTE, image.data)

    -- Tile support: allows drawing of fixed-size sub-images.
    local tw = tile_w and tile_w or image.width
    local th = tile_h and tile_h or image.height
    if tw < 1 or th < 1 then
        error("Cannot tile sprite by width or height < 1 (got w = " .. tw .. ", h = " .. th .. ")")
    end

    -- Standard Lua OOP setup.
    local s = {
        texture_id = texture_id,
        texture_alpha = use_alpha,
        w = image.width,
        h = image.height,
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
    gl.TexEnv(gl.TEXTURE_ENV, gl.TEXTURE_ENV_MODE, gl.REPLACE)

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
    local use_alpha = self.texture_alpha
    gl.Enable(gl.TEXTURE_2D)
        gl.BlendFunc(gl.SRC_ALPHA, gl.ONE)
        if use_alpha then gl.Enable(gl.BLEND) end
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
        if use_alpha then gl.Disable(gl.BLEND) end
    gl.Disable(gl.TEXTURE_2D)
end
