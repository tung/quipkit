local path, entry
if WIN32 then
    path = "lib/sdl/luaSDL/luaSDL.dll"
    entry = "luaopen_SDL"
else
    -- Assume Linux for now.
    path = "lib/sdl/LuaSDL_new/libluasdl.so"
    entry = "luaopen_LuaSDL"
end
local f = assert(package.loadlib(path, entry))
f()

if WIN32 then
    -- Old luaSDL provides high- and low-level BlitSurface-esque functions.
    -- The high-level one is essentially the one we want.
    SDL.SDL_BlitSurface = SDL.SDL_UpperBlit
end
