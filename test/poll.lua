require "sdl"

if SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end

local function main()
    SDL.SDL_SetVideoMode(256, 256, 16, 0)

    local done = false
    local event = SDL.SDL_Event_new()
    while not done do
        if SDL.SDL_PollEvent(event) then
            if event.type == SDL.SDL_MOUSEMOTION then
                print("mouse motion - pos ", event.motion.x, event.motion.y,
                    " - change ", event.motion.xrel, event.motion.yrel)
            elseif event.type == SDL.SDL_MOUSEBUTTONDOWN then
                print("mouse press  - button ", event.button.button,
                    " - at ", event.button.x, event.button.y)
            elseif event.type == SDL.SDL_QUIT then
                print("bye")
                done = true
            end
        end
    end
end

local success, error_message = pcall(main)
SDL.SDL_Quit()
if not success then
    error(error_message)
end
