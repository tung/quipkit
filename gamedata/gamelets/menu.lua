-- A simple menu that shows menu items as a certain number of blobs,
-- and a cursor as a single blob.
-- When Enter is pressed, switch to the center_image gamelet.

require "game"
require "sdl"
require "sprite"

module(..., package.seeall)

MT = { __index = getfenv() }

function new(g, o)
    local inst = o or {}

    -- Ensure all items are 1+ in length.
    for i, v in ipairs(inst.items) do
        if v < 1 then
            inst.items[i] = 1
        end
    end

    -- Set default menu selection.
    if inst.default ~= nil then
        inst.selection = inst.default
    else
        inst.selection = 1
    end
    if inst.selection < 1 then inst.selection = 1 end
    if inst.selection > #inst.items then inst.selection = #inst.items end

    inst.blob = sprite:new("test/glob.png", 32, 32)

    setmetatable(inst, MT)
    return inst
end

function event(self, e)
    if e.type == SDL.SDL_KEYDOWN then
        local key = e.key.keysym.sym
        if key == SDL.SDLK_DOWN then
            self.selection = self.selection + 1
            if self.selection > #self.items then self.selection = 1 end
        elseif key == SDL.SDLK_UP then
            self.selection = self.selection - 1
            if self.selection < 1 then self.selection = #self.items end
        elseif key == SDL.SDLK_RETURN then
            table.remove(gamelet.stack)
            table.remove(gamelet.stack)
            table.insert(gamelet.stack, gamelet("center_image"):new{
                image = "test/smile.png",
                scale = 8.0
            })
            table.insert(gamelet.stack, gamelet("top_text"):new{
                font = "test/bitstream-vera-sans-bold-24pt.png",
                text = "Image again"
            })
        elseif key == SDL.SDLK_ESCAPE then
            return game.QUIT
        else
            return
        end
        return game.REDRAW

    elseif e.type == SDL.SDL_QUIT then
        return game.QUIT
    elseif e.type == SDL.SDL_VIDEOEXPOSE or e.type == SDL.SDL_ACTIVEEVENT then
        return game.REDRAW
    end
end

function update(self, delta)
    -- No update needed.
end

function draw(self)
    -- We'll draw the menu at the top-left for simplicity.
    -- Selection gets a column, then a blank, then the item.
    -- Everything is blobs because I don't have text support yet. ;_;

    -- Draw selection cursor.
    self.blob:draw(0, (self.selection - 1) * self.blob.tile_h)

    -- Draw the items.
    for i in ipairs(self.items) do
        for x = 1, self.items[i] do
            self.blob:draw(self.blob.tile_w * (1 + x), self.blob.tile_h *  (i - 1))
        end
    end
end
