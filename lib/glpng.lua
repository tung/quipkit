local path = "/home/tung/Projects/quipkit/lib/glpng/glpng.so"
local entry = "luaopen_glpng"
local f = assert(package.loadlib(path, entry))
f()
