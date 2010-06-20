require "sdl"
require "png"

RMASK = 0x000000ff
GMASK = 0x0000ff00
BMASK = 0x00ff0000
AMASK = 0xff000000

if SDL.SDL_Init(SDL.SDL_INIT_VIDEO) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end

local function main()
    local screen = SDL.SDL_SetVideoMode(640, 480, 32, SDL.SDL_SWSURFACE)
    local smile_png = PNG.Open("test/smile.png")
    local smile_surface
    if smile_png.format == PNG.RGB then
        smile_surface = SDL.SDL_CreateRGBSurfaceFrom(smile_png.data_ptr, smile_png.width, smile_png.height, 24, smile_png.width * 3, RMASK, GMASK, BMASK, 0)
    else
        smile_surface = SDL.SDL_CreateRGBSurfaceFrom(smile_png.data_ptr, smile_png.width, smile_png.height, 32, smile_png.width * 4, RMASK, GMASK, BMASK, AMASK)
    end
    if not smile_surface then
        error("SDL_CreateRGBSurfaceFrom failed: " .. SDL.SDL_GetError())
    end

    local dest_rect = SDL.SDL_Rect_new()
    dest_rect.x = 320
    dest_rect.y = 240
    SDL.SDL_BlitSurface(smile_surface, nil, screen, dest_rect)
    SDL.SDL_Flip(screen)
    SDL.SDL_Delay(3000)

    SDL.SDL_FreeSurface(smile_surface)
end

local success, error_message = pcall(main)
SDL.SDL_Quit()
if not success then
    error(error_message)
end
