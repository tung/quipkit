local path = "lib/png/png.so"
local entry = "luaopen_png"
local f = assert(package.loadlib(path, entry))
f()
