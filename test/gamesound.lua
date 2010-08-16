require "game"
require "proAudioRt"

function game.init()
    local sample = proAudio.sampleFromFile("test/sample.ogg")
    proAudio.soundPlay(sample)
end

function game.update(delta)
    if proAudio.soundActive() == 0 then
        return game.QUIT
    end
    proAudio.sleep(0.05)
end

game.setScreenSize(320, 240)
game.setScreenTitle("Sound Demo with game.lua")
game.run()
