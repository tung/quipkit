-- A simple menu that shows menu items as a certain number of blobs,
-- and a cursor as a single blob.
-- When Enter is pressed, switch to the center_image gamelet.

require "game"
require "sprite"

require "sdl"

gamelet.new(...)

function init(opts)
    local self = gamelet.instance()

    -- Ensure all items are 1+ in length.
    self.items = {}
    for i in ipairs(opts.items) do
        if opts.items[i] > 0 then
            self.items[i] = opts.items[i]
        else
            self.items[i] = 1
        end
    end

    -- Set default menu selection.
    local num_items = #(self.items)
    if opts.default < 1 then
        self.selection = 1
    elseif opts.default > num_items then
        self.selection = num_items
    else
        self.selection = opts.default
    end

    self.blob = sprite:new("test/glob.png", 32, 32)

    return self
end

function event(self, e)
    if e.type == SDL.SDL_KEYDOWN then
        local key = e.key.keysym.sym
        if key == SDL.SDLK_DOWN then
            self.selection = self.selection + 1
            if self.selection > #(self.items) then
                self.selection = 1
            end
        elseif key == SDL.SDLK_UP then
            self.selection = self.selection - 1
            if self.selection < 1 then
                self.selection = #(self.items)
            end
        elseif key == SDL.SDLK_RETURN then
            gamelet.stack:pop()
            gamelet.stack:pop()
            gamelet.stack:push(gamelet.load("center_image").init{
                image = "test/smile.png",
                scale = 8.0
            })
            gamelet.stack:push(gamelet.load("top_text").init{
                font = "test/bitstream-vera-sans-bold-24pt.png",
                text = "Image again"
            })
        elseif key == SDL.SDLK_ESCAPE then
            return gamelet.GAMELET_QUIT
        else
            return
        end
        return gamelet.GAMELET_REDRAW

    elseif e.type == SDL.SDL_QUIT then
        return gamelet.GAMELET_QUIT
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
