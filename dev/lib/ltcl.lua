local path = "dev/lib/tcltk/ltcltk-0.9-1/ltcl.so"
local entry = "luaopen_ltcl"
local f = assert(package.loadlib(path, entry))
return f()
