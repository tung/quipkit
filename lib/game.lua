--[[ See copyright notice in doc/COPYRIGHT. ]]

require 'sdl'

local SDL = SDL

local string = string

module(...)



-- yield the CPU for at least ms milliseconds
function sleep(ms)
    SDL.SDL_Delay(ms)
end


-- Map of SDL key constants to Quipkit key strings.
local key_strings
do
    -- k('blah') == SDL.SDLK_BLAH
    local function k(name)
        return SDL["SDLK_" .. string.upper(name)]
    end
    key_strings = {
        [k('up')] = 'up',
        [k('down')] = 'down',
        [k('left')] = 'left',
        [k('right')] = 'right',
    }
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
    if s.type == SDL.SDL_KEYDOWN then
        e.type = 'keydown'
        e.key = key_strings[s.key.keysym.sym]
    elseif s.type == SDL.SDL_KEYUP then
        e.type = 'keyup'
        e.key = key_strings[s.key.keysym.sym]
    elseif s.type == SDL.SDL_QUIT then
        e.type = 'exit'
    end

    return e
end



return _M
