require "sdl"
require "sfont"



local function init()
    font = sfont.sdlFont:new("test/bitstream-vera-sans-bold-24pt.png")
end


local function draw(screen)
    if SDL.SDL_FillRect(screen, nil, SDL.SDL_MapRGB(screen.format, 0, 0, 0)) == -1 then
        error("SDL_FillRect failed: " .. SDL.SDL_GetError())
    end
    font:write(screen, 0, 0, "SFont works in Quipkit!")
    if SDL.SDL_Flip(screen) == -1 then
        error("SDL_Flip failed: " .. SDL.SDL_GetError())
    end
end


local function main()
    local screen = SDL.SDL_SetVideoMode(320, 240, 32, 0)

    init()
    draw(screen)

    local event = SDL.SDL_Event_local()
    while SDL.SDL_WaitEvent(event) == 1 do
        local t = event.type
        if t == SDL.SDL_QUIT then
            break
        elseif t == SDL.SDL_ACTIVEEVENT or t == SDL.SDL_VIDEOEXPOSE then
            draw(screen)
        end
    end
end



if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end

local success, error_message = pcall(main)
SDL.SDL_Quit()
if not success then
    print(error_message)
end
