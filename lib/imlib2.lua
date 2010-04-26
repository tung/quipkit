local path = "/home/tung/Projects/quipkit/lib/imlib2/lua-imlib2-0.1/limlib2.so"
local entry = "luaopen_limlib2"
local f = assert(package.loadlib(path, entry))
f()
