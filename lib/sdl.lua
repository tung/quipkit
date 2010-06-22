local path, entry
if WIN32 then
    path = "lib/sdl/luaSDL/luaSDL.dll"
    entry = "luaopen_SDL"
else
    -- Assume Linux for now.
    path = "lib/sdl/luaSDL/libluasdl.so"
    entry = "luaopen_SDL"
end
local f = assert(package.loadlib(path, entry))
f()

-- luaSDL provides high- and low-level BlitSurface-esque functions.
-- The high-level one is essentially the one we want.
SDL.SDL_BlitSurface = SDL.SDL_UpperBlit
