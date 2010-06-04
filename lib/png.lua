local path
if WIN32 then
    path = "lib/png/bin/Debug/luapng.dll"
else
    -- Assume Linux for now.
    path = "lib/png/png.so"
end
local entry = "luaopen_png"
local f = assert(package.loadlib(path, entry))
f()
