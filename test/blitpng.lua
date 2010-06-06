require "sdl"
require "png"

RMASK = 0x000000ff
GMASK = 0x0000ff00
BMASK = 0x00ff0000
AMASK = 0xff000000

assert(SDL.Init(SDL.INIT_VIDEO))

local function main()
    local screen = SDL.SetVideoMode(640, 480, 32, SDL.SWSURFACE)
    local smile_png = PNG.Open("test/smile.png")
    local smile_surface
    if smile_png.format == PNG.RGB then
        -- Note: pitch seems to be the number of BYTES per scanline, not pixels.
        smile_surface = SDL.CreateRGBSurfaceFrom(smile_png.data, smile_png.width, smile_png.height, 24, smile_png.width * 3, RMASK, GMASK, BMASK, 0)
    else
        smile_surface = SDL.CreateRGBSurfaceFrom(smile_png.data, smile_png.width, smile_png.height, 32, smile_png.width * 4, RMASK, GMASK, BMASK, AMASK)
    end
    SDL.BlitSurface(smile_surface, nil, screen, {x = 320, y = 240})
    SDL.Flip(screen)
    SDL.Delay(3000)
    SDL.FreeSurface(smile_surface)
end

SDL.Assert(pcall(main))
SDL.Quit()
