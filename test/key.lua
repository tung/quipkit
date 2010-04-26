require "sdl"

assert(SDL.Init(SDL.INIT_VIDEO))

local function main()
    local event, keysym

    SDL.SetVideoMode(256, 256, 16, 0)
    print("Press 'Q' to quit.")

    event = SDL.WaitEvent()
    while true do
        if event.type == SDL.KEYDOWN then
            keysym = event.key.keysym
            print(" pressed sym", keysym.sym, SDL.GetKeyName(keysym.sym), keysym.mod[KMOD.LSHIFT])
            if keysym.sym == SDLK.q then
                print("'Q' pressed. Bye.")
                return
            end

        elseif event.type == SDL.KEYUP then
            keysym = event.key.keysym
            print("released sym", keysym.sym, SDL.GetKeyName(keysym.sym), keysym.mod[KMOD.LSHIFT])

        elseif event.type == SDL.QUIT then
            print("Quit event. Bye.")
            return
        end

        event = SDL.WaitEvent()
    end
end

SDL.Assert(pcall(main))
SDL.Quit()
