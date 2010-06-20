require "sdl"
require "gl"

if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end

local function main()
    -- Create OpenGL rendering context.
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DOUBLEBUFFER, 1)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_RED_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_GREEN_SIZE, 8)
    SDL.SDL_GL_SetAttribute(SDL.SDL_GL_BLUE_SIZE, 8)
    SDL.SDL_SetVideoMode(640, 480, 32, SDL.SDL_OPENGL)

    SDL.SDL_WM_SetCaption("OpenGL with SDL", "OpenGL")

    -- Tell OpenGL where to draw in the window.
    gl.Viewport(80, 0, 480, 480)

    -- Prepare a perspective projection and viewing volume,
    -- similar to picking and adjusting the camera lens.
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    gl.Frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0)

    -- Draw the scene.
    -- We could move and point the camera here if we wanted,
    -- but by default it starts at (0, 0, 0) and points down the -z axis.
    gl.MatrixMode(gl.MODELVIEW)
    gl.LoadIdentity()

    -- gl flags are combined with a string with the flag names,
    -- not a table like with SDL.
    gl.ClearColor(0.0, 0.0, 0.0, 0.0)
    gl.Clear(gl.COLOR_BUFFER_BIT)

    -- (0, 0) is at the BOTTOM-left, and the +y axis points UP the screen.
    gl.Begin(gl.TRIANGLES)
        gl.Color(1.0, 0.0, 0.0)
        gl.Vertex(0.0, 1.0, -2.0)
        gl.Color(0.0, 1.0, 0.0)
        gl.Vertex(1.0, -1.0, -2.0)
        gl.Color(0.0, 0.0, 1.0)
        gl.Vertex(-1.0, -1.0, -2.0)
    gl.End()
    gl.Flush()

    SDL.SDL_GL_SwapBuffers()

    SDL.SDL_Delay(5000)
end

local success, error_message = pcall(main)
SDL.SDL_Quit()
if not success then
    error(error_message)
end
