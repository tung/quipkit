require "sound"

sound.init()

local sample = sound.sample:new("test/sample.ogg")
sample:play()

while sound.numActive() > 0 do
    sound.sleep(0.05)
end

sound.close()
