local path
if WIN32 then
    -- TODO: Support Release target too.
    path = "lib/gl/luagl/bin/Debug/luaglu.dll"
else
    -- Assume Linux for now.
    path = "lib/gl/luagl/libluaglu.so"
end
local entry = "luaopen_luaglu"
local f = assert(package.loadlib(path, entry))
f()
