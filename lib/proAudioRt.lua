local path
if WIN32 then
    path = "lib/proteaAudio/proAudioRt.dll"
else
    -- Assume Linux for now.
    path = "lib/proteaAudio/libproaudio.so"
end
local entry = "luaopen_proAudioRt"
local f = assert(package.loadlib(path, entry))
f()
