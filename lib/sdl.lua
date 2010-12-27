local path
if WIN32 then
    -- TODO: Support Release target too.
    path = "lib/sdl/LuaSDL_new/bin/Debug/LuaSDL.dll"
else
    -- Assume Linux for now.
    path = "lib/sdl/LuaSDL_new/libluasdl.so"
end
local entry = "luaopen_LuaSDL"
local f = assert(package.loadlib(path, entry))
f()
