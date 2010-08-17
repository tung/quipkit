require "sdl"

local function init()
    font = SDL.TTF_OpenFont("test/Vera.ttf", 24)
    if not font then
        error("TTF_OpenFont failed: " .. SDL.TTF_GetError())
    end
end

local function draw(screen)
    if SDL.SDL_FillRect(screen, nil, SDL.SDL_MapRGB(screen.format, 0, 0, 0)) == -1 then
        error("SDL_FillRect failed: " .. SDL.SDL_GetError())
    end

    local white = SDL.SDL_Color_local()
    white.r = 255
    white.g = 255
    white.b = 255
    --local tmp = SDL.TTF_RenderText_Blended(font, "SDL_ttf is working!", white)
    local tmp = SDL.TTF_RenderText_Blended(font, "SDL_ttf is working!", white)
    if not tmp then
        error("TTF_RenderText_Blended failed: " .. SDL.TTF_GetError())
    end
    local dest = SDL.SDL_Rect_local()
    dest.x = 0
    dest.y = 0
    if SDL.SDL_BlitSurface(tmp, nil, screen, dest) ~= 0 then
        SDL.SDL_FreeSurface(tmp)
        error("SDL_BlitSurface failed: " .. SDL.SDL_GetError())
    end
    SDL.SDL_FreeSurface(tmp)

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

    SDL.TTF_CloseFont(font)
end

if SDL.SDL_Init(SDL.SDL_INIT_VIDEO) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end
if SDL.TTF_Init() == -1 then
    error("TTF_Init failed: " .. SDL.TTF_GetError())
end

local success, error_message = pcall(main)
SDL.TTF_Quit()
SDL.SDL_Quit()
if not success then
    print(error_message)
end
