local path
if WIN32 then
    path = "lib/sdlgl/sdlgl.dll"
else
    -- Assume Linux for now.
    path = "lib/sdlgl/libsdlgl.so"
end
local entry = "luaopen_sdlgl"
local f = assert(package.loadlib(path, entry))
f()
