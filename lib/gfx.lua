--[[ See copyright notice in doc/COPYRIGHT. ]]

require 'gl'
require 'sdl'

local gl = gl
local SDL = SDL

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



return _M
