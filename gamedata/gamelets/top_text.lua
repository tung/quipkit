-- Show text centered at the top.
-- Passes event and update calls through.

require "game"
require "sfont"

module(..., package.seeall)

MT = { __index = getfenv() }

function new(g, o)
    local inst = o or {}
    inst.font = sfont.glFont:new(inst.font)
    setmetatable(inst, MT)
    return inst
end

function event(self, e)
    return gamelet.NEXT
end

function update(self, delta)
    return gamelet.NEXT
end

function draw(self)
    local screen_w, screen_h = game.getScreenSize()
    local x = (screen_w - self.font:textWidth(self.text)) / 2
    self.font:write(x, 0, self.text)
    return gamelet.NEXT
end
