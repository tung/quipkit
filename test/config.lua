require "game"
require "sfont"
require "sprite"

local option
local font
local cursor

function game.init()
    local screen_w, screen_h = game.getScreenSize()
    if screen_w == 640 then
        option = 1
    else
        option = 2
    end
    font = sfont.glFont:new("test/bitstream-vera-sans-bold-24pt.png")
    cursor = sprite:new("test/smile.png")
end

function game.event(e)
    local quit = false

    if e.type == SDL.SDL_KEYDOWN then
        local key = e.key.keysym.sym
        if key == SDL.SDLK_UP then
            option = 1
        elseif key == SDL.SDLK_DOWN then
            option = 2
        elseif key == SDL.SDLK_RETURN or key == SDL.SDLK_ESCAPE then
            quit = true
        end
        if key == SDL.SDLK_UP or key == SDL.SDLK_DOWN then
            return game.REDRAW
        end
    elseif e.type == SDL.SDL_QUIT then
        quit = true
    end

    if quit then
        if option == 1 then
            game.setScreenSize(640, 480)
        elseif option == 2 then
            game.setScreenSize(320, 240)
        end
        game.saveConfig()
        return game.QUIT
    end
end

function game.draw()
    game.clearScreen()

    font:write(0, 0, "Next time, open in...")
    font:write(32, 64, "640 * 480")
    font:write(32, 96, "320 * 240")

    if option == 1 then
        cursor:draw(0, 64)
    elseif option == 2 then
        cursor:draw(0, 96)
    end
end

game.loadConfig()
game.run()
