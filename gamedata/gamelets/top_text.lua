-- Show text centered at the top.
-- Passes event and update calls through.

require "game"
require "sfont"

gamelets.new(...)

function new(self, opts)
    local inst = gamelets.instance(self)
    inst.font = sfont.glFont:new(opts.font)
    inst.text = opts.text
    return inst
end

function event(self, e)
    return gamelets.GAMELET_NEXT
end

function update(self, delta)
    return gamelets.GAMELET_NEXT
end

function draw(self)
    local screen_w, screen_h = game.getScreenSize()
    local x = (screen_w - self.font:textWidth(self.text)) / 2
    self.font:write(x, 0, self.text)
    return gamelets.GAMELET_NEXT
end
