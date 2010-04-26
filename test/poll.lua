require "sdl"

assert(SDL.Init(SDL.INIT_EVERYTHING))

local function main()
    SDL.SetVideoMode(256, 256, 16, 0)

    local done = false
    local event = SDL.PollEvent()
    while true do
        if event then
            if event.type == SDL.MOUSEMOTION then
                print("mouse motion - pos ", event.motion.x, event.motion.y,
                    " - change ", event.motion.xrel, event.motion.yrel)
            elseif event.type == SDL.MOUSEBUTTONDOWN then
                print("mouse press  - button ", event.button.button,
                    " - at ", event.button.x, event.button.y)
            elseif event.type == SDL.QUIT then
                print("bye")
                return
            end
        end
        event = SDL.PollEvent()
    end
end

SDL.Assert(pcall(main))

SDL.Quit()
