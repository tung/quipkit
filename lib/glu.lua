local path
if WIN32 then
    error("'glu' package not built for Windows yet")
else
    -- Assume Linux for now.
    path = "lib/gl/luagl/libluaglu.so"
end
local entry = "luaopen_luaglu"
local f = assert(package.loadlib(path, entry))
f()
