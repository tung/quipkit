local path
if WIN32 then
    -- TODO: Support Release target too.
    path = "lib/sdlgl/bin/Debug/sdlgl.dll"
else
    -- Assume Linux for now.
    path = "lib/sdlgl/libsdlgl.so"
end
local entry = "luaopen_sdlgl"
local f = assert(package.loadlib(path, entry))
f()
