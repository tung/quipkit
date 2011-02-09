--[[ See copyright notice in doc/COPYRIGHT. ]]

local path
if WIN32 then
    path = "lib/gl/luagl/LuaGLU.dll"
else
    -- Assume Linux for now.
    path = "lib/gl/luagl/libluaglu.so"
end
local entry = "luaopen_luaglu"
local f = assert(package.loadlib(path, entry))
f()
