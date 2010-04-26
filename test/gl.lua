require "sdl"
require "gl"
print("good")

assert(SDL.Init(SDL.INIT_EVERYTHING))
print("better")

local function main()
    -- Create OpenGL rendering context.
    SDL.GL.SetAttribute(SDL.GL.DOUBLEBUFFER, 1)
    SDL.GL.SetAttribute(SDL.GL.RED_SIZE, 8)
    SDL.GL.SetAttribute(SDL.GL.GREEN_SIZE, 8)
    SDL.GL.SetAttribute(SDL.GL.BLUE_SIZE, 8)
    SDL.SetVideoMode(640, 480, 32, SDL.OPENGL)

    SDL.WM.SetCaption("OpenGL with SDL", "OpenGL")

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

    SDL.GL.SwapBuffers()

    SDL.Delay(5000)
end

SDL.Assert(pcall(main))

SDL.Quit()
print("best")
