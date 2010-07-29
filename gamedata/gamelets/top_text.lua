-- Show text centered at the top.
-- Passes event and update calls through.

require "game"
require "sfont"

gamelet.new(...)

function init(opts)
    local self = gamelet.instance()
    self.font = sfont.glFont:new(opts.font)
    self.text = opts.text
    return self
end

function event(self, e)
    return gamelet.GAMELET_NEXT
end

function update(self, delta)
    return gamelet.GAMELET_NEXT
end

function draw(self)
    local screen_w, screen_h = game.getScreenSize()
    local x = (screen_w - self.font:textWidth(self.text)) / 2
    self.font:write(x, 0, self.text)
    return gamelet.GAMELET_NEXT
end
