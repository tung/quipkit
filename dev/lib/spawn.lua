local path
if WIN32 then
    error("spawn library not supported on Windows yet")
else
    -- Assume Linux for now.
    path = "dev/lib/spawn/libspawn.so"
end
local entry = "luaopen_spawn"
local f = assert(package.loadlib(path, entry))
f()
