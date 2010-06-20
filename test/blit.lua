require "sdl"

if SDL.SDL_Init(SDL.SDL_INIT_VIDEO) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end

local function main()
    local screen = SDL.SDL_SetVideoMode(640, 480, 32, SDL.SDL_SWSURFACE)
    local smile_bmp = SDL.SDL_LoadBMP("test/smile.bmp")

    local src_rect = SDL.SDL_Rect_new()
    src_rect.x = 8
    src_rect.y = 8
    src_rect.w = 16
    src_rect.h = 16
    local dest_rect = SDL.SDL_Rect_new()
    dest_rect.x = 320
    dest_rect.y = 240

    SDL.SDL_BlitSurface(smile_bmp, src_rect, screen, dest_rect)
    SDL.SDL_Flip(screen)
    SDL.SDL_Delay(3000)

    SDL.SDL_FreeSurface(smile_bmp)
end

local success, error_message = pcall(main)
SDL.SDL_Quit()
if not success then
    error(error_message)
end
