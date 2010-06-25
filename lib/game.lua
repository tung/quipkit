-- Quipkit game engine framework.
-- Loads SDL and OpenGL, and drives the game loop.

require "sdl"
require "gl"
require "png"

module(..., package.seeall)


-- Main Framework Stuff
Events = {}

Screen = {w = 640, h = 480, title = "Quipkit"}

-- Called after SDL.Init().
Init = function ()
end

-- Called before SDL.Exit().
Exit = function ()
end

Update = function (delta)
end

Draw = function ()
end

-- Returns 'false' if any handler returns it.
-- This lets the user end the game loop.
local event
local function handle_events()
    while SDL.SDL_PollEvent(event) == 1 do
        local handler = Events[event.type]
        if type(handler) == "function" then
            if handler() == false then
                return false
            end
        end
    end
    return true
end

local function loop()
    local last, this
    last = SDL.SDL_GetTicks()
    while handle_events() do
        this = SDL.SDL_GetTicks()
        if Update(this - last) == false then
            -- Allow Update() to end the game loop.
            return
        end
        last = this

        gl.MatrixMode(gl.MODELVIEW)
        gl.LoadIdentity()
        Draw()
        gl.Flush()
        SDL.SDL_GL_SwapBuffers()
    end
end

Run = function ()
    -- Is this a good way of going about things?
    if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
        error("Failed to initialize SDL: " .. SDL.SDL_GetError(), 2)
    end

    SDL.SDL_WM_SetCaption(Screen.title, "")

    -- TODO: Allow customisation of these.
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DOUBLEBUFFER, 1)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_RED_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_GREEN_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_BLUE_SIZE, 8)
    SDL.SDL_SetVideoMode(Screen.w, Screen.h, 32, SDL.SDL_OPENGL)

    -- Accept byte-aligned textures.
    gl.PixelStore(gl.UNPACK_ALIGNMENT, 1)

    -- Tell OpenGL where to render in our window.
    gl.Viewport(0, 0, Screen.w, Screen.h)

    -- Project the scenes-to-be orthographically, like in blue-prints.
    -- Also, switch bottom and top so (0, 0) is top-left and y extends down.
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Ortho(0, Screen.w, Screen.h, 0, -1, 1)

    Init()

    event = SDL.SDL_Event_new()

    local success, error_message
    success, error_message = pcall(loop)
    if not success then
        SDL.SDL_Quit()
        error("Error in game loop: " .. error_message, 2)
    end

    Exit()
    SDL.SDL_Quit()
end


--- Utility Functions
ClearScreen = function ()
    gl.ClearColor(0, 0, 0, 0)
    gl.Clear(gl.COLOR_BUFFER_BIT)
end


--- Sprite Class
Sprite = {}

function Sprite:new(image_file, tile_w, tile_h)
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
    setmetatable(s, self)
    self.__index = self
    return s
end

function Sprite:SetTile(tile_x, tile_y)
    self.tile_x = tile_x
    self.tile_y = tile_y
end

function Sprite:Draw(x, y)
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
