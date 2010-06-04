local path
if WIN32 then
    path = "lib/gl/luagl1_3_Win32_dll8_lib/luagl.dll"
else
    -- Assume Linux for now.
    path = "lib/gl/luagl1_3_Linux26g4_lib/libluagl.so"
end
local entry = "luaopen_luagl"
local f = assert(package.loadlib(path, entry))
f()
