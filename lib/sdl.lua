local path
if WIN32 then
    path = "lib/sdl/LuaSDL_new/LuaSDL.dll"
else
    -- Assume Linux for now.
    path = "lib/sdl/LuaSDL_new/libluasdl.so"
end
local entry = "luaopen_LuaSDL"
local f = assert(package.loadlib(path, entry))
f()
