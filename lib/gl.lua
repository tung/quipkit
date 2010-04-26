local path = "/home/tung/Projects/quipkit/lib/gl/luagl1_3_Linux26g4_lib/libluagl.so"
local entry = "luaopen_luagl"
local f = assert(package.loadlib(path, entry))
f()
