local path = "/home/tung/Projects/quipkit/lib/gl_old/gl.so"
local entry = "luaopen_gl"
local f = assert(package.loadlib(path, entry))
f()
