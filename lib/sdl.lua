local path
if WIN32 then
    path = "lib/sdl/bin/Debug/luasdl.dll"
else
    -- Assume Linux for now.
    path = "lib/sdl/libluasdl.so"
end
local entry = "luaopen_luasdl"
local f = assert(package.loadlib(path, entry))
f()
