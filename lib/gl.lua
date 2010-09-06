local path
if WIN32 then
    path = "lib/gl/luagl/luagl.dll"
else
    -- Assume Linux for now.
    path = "lib/gl/luagl/libluagl.so"
end
local entry = "luaopen_luagl"
local f = assert(package.loadlib(path, entry))
f()
