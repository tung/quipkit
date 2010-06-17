local path
if WIN32 then
    path = "dev/lib/tcltk/ltcltk-0.9-1/bin/Debug/ltcl.dll"
else
    -- Assume Linux for now.
    path = "dev/lib/tcltk/ltcltk-0.9-1/ltcl.so"
end
local entry = "luaopen_ltcl"
local f = assert(package.loadlib(path, entry))
return f()
