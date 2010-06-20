-- Quipkit game engine framework.
-- Loads SDL and OpenGL, and drives the game loop.

require "sdl"
require "gl"
require "png"

module(..., package.seeall)


-- Main Framework Stuff
Events = {}

Screen = {w = 640, h = 480}

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

function Sprite:new(image_file)
    -- Load image into OpenGL texture.
    local image = PNG.Open(image_file)
    local texture_id = gl.GenTextures(1)[1]
    gl.BindTexture(gl.TEXTURE_2D, texture_id)
    -- Tell OpenGL how to stretch/shrink our texture while we're at it.
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
    gl.TexParameter(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
    local gl_format = gl.RGBA
    local data_format = (image.format == PNG.RGBA) and gl.RGBA or gl.RGB
    gl.TexImage2D(0, gl_format, image.width, image.height, 0, data_format, gl.UNSIGNED_BYTE, image.data)

    -- Standard Lua OOP setup.
    local s = {
        texture_id = texture_id,
        w = image.width,
        h = image.height
    }
    setmetatable(s, self)
    self.__index = self
    return s
end

function Sprite:Draw(x, y)
    -- Replace/draw over existing fragments (pixels).
    gl.TexEnv(gl.TEXTURE_ENV, gl.TEXTURE_ENV_MODE, gl.REPLACE)

    -- Move the sprite where it needs to be.
    gl.LoadIdentity()
    gl.Translate(x, y, 0)

    -- Draw the sprite.
    gl.Enable(gl.TEXTURE_2D)
    gl.BindTexture(gl.TEXTURE_2D, self.texture_id)
    gl.Begin(gl.QUADS)
        gl.TexCoord(0, 0); gl.Vertex(     0,      0)
        gl.TexCoord(1, 0); gl.Vertex(self.w,      0)
        gl.TexCoord(1, 1); gl.Vertex(self.w, self.h)
        gl.TexCoord(0, 1); gl.Vertex(     0, self.h)
    gl.End()
    gl.Disable(gl.TEXTURE_2D)
end
