require "sdl"

assert(SDL.Init(SDL.INIT_VIDEO))

local function main()
    local screen = SDL.SetVideoMode(640, 480, 32, SDL.SWSURFACE)
    local smile_bmp = SDL.LoadBMP("test/smile.bmp")
    SDL.BlitSurface(smile_bmp, {x = 8, y = 8, w = 16, h = 16}, screen, {x = 320, y = 240})
    SDL.Flip(screen)
    SDL.Delay(3000)
    SDL.FreeSurface(smile_bmp)
end

SDL.Assert(pcall(main))
SDL.Quit()
