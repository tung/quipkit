--[[ See copyright notice in doc/COPYRIGHT. ]]

require 'sdl'

local SDL = SDL

module(...)



-- yield the CPU for at least ms milliseconds
function sleep(ms)
    SDL.SDL_Delay(ms)
end


-- SDL_Event userdatum so we're not constantly making/destroying them.
-- Used in game.event.
local m_sdl_ev = SDL.SDL_Event_local()

-- poll (or wait for) and return the next queued input event
function event(wait)
    if wait == nil then wait = false end

    -- Grab the SDL event.
    if wait then
        if SDL.SDL_WaitEvent(m_sdl_ev) == 0 then
            error("SDL_WaitEvent: " .. SDL.SDL_GetError())
        end
    else
        if SDL.SDL_PollEvent(m_sdl_ev) == 0 then
            return nil
        end
    end

    -- Translate the SDL event.
    local s = m_sdl_ev
    local e = {}
    if s.type == SDL.SDL_QUIT then
        e.type = "exit"
    end

    return e
end



return _M
