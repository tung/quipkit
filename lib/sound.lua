-- Quipkit sound package.
-- A high-level API for loading and playing audio.

require "proAudioRt"

module(..., package.seeall)



function init()
    if not proAudio.create() then
        error("Could not initialize audio device.")
    end
end


-- Return the number of active sounds.
function numActive()
    return proAudio.soundActive()
end


-- Suspend the current thread, letting audio play in a background thread.
function sleep(seconds)
    proAudio.sleep(seconds)
end


function close()
    proAudio.destroy()
end



--- sound.sample object

sample = {}


-- Create a new sound.sample object.
function sample:new(path)
    local o = {}
    o.path = path
    o.handle = proAudio.sampleFromFile(path)
    self.__index = self
    setmetatable(o, self)
    return o
end


function sample:play()
    if proAudio.soundPlay(self.handle) == -1 then
        error("Error playing \"" .. self.path .. "\".")
    end
end
