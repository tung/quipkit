require "sdl"

if SDL.SDL_Init(SDL.SDL_INIT_VIDEO) == -1 then
    error("SDL_Init failed: " .. SDL.SDL_GetError())
end

local function mod_to_string(sdlmod)
    local str = {}
    if SDL.bit_and(sdlmod, SDL.KMOD_LSHIFT) ~= 0 then str[#str + 1] = "LSHIFT"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_RSHIFT) ~= 0 then str[#str + 1] = "RSHIFT"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_LCTRL) ~= 0 then str[#str + 1] = "LCTRL"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_RCTRL) ~= 0 then str[#str + 1] = "RCTRL"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_LALT) ~= 0 then str[#str + 1] = "LALT"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_RALT) ~= 0 then str[#str + 1] = "RALT"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_LMETA) ~= 0 then str[#str + 1] = "LMETA"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_RMETA) ~= 0 then str[#str + 1] = "RMETA"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_NUM) ~= 0 then str[#str + 1] = "NUM"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_CAPS) ~= 0 then str[#str + 1] = "CAPS"
    elseif SDL.bit_and(sdlmod, SDL.KMOD_MODE) ~= 0 then str[#str + 1] = "MODE"
    end
    return (#str == 0) and "no mods" or table.concat(str, " ")
end

local function main()
    local event = SDL.SDL_Event_new()
    local keysym

    SDL.SDL_SetVideoMode(256, 256, 16, 0)
    print("Press 'Q' to quit.")

    local wait_status = SDL.SDL_WaitEvent(event)
    while true do
        if wait_status == 0 then
            error("SDL_WaitEvent failed: " .. SDL.SDL_GetError())
        end

        if event.type == SDL.SDL_KEYDOWN then
            keysym = event.key.keysym
            print(" pressed sym", keysym.sym, SDL.SDL_GetKeyName(keysym.sym), mod_to_string(keysym.mod))
            if keysym.sym == SDL.SDLK_q then
                print("'Q' pressed. Bye.")
                return
            end

        elseif event.type == SDL.SDL_KEYUP then
            keysym = event.key.keysym
            print("released sym", keysym.sym, SDL.SDL_GetKeyName(keysym.sym), mod_to_string(keysym.mod))

        elseif event.type == SDL.SDL_QUIT then
            print("Quit event. Bye.")
            return
        end

        wait_status = SDL.SDL_WaitEvent(event)
    end
end

local success, error_message = pcall(main)
SDL.SDL_Quit()
if not success then
    error(error_message)
end
