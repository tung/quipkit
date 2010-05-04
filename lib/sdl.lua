local path = "lib/sdl/sdl.so"
local entry = "luaopen_sdl"
local f = assert(package.loadlib(path, entry))
f()
