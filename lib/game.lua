--[[ See copyright notice in doc/COPYRIGHT. ]]

require 'sdl'

local SDL = SDL

module(...)



-- yield the CPU for at least ms milliseconds
function sleep(ms)
    SDL.SDL_Delay(ms)
end


-- Map of SDL key constants to Quipkit key strings.
local key_strings
do
    -- k('BLAH') == SDL.SDLK_BLAH
    local function k(name)
        return SDL["SDLK_" .. name]
    end
    key_strings = {
        [k('BACKSPACE')] = 'backspace',
        [k('TAB')] = 'tab',
        [k('CLEAR')] = 'clear',
        [k('RETURN')] = 'enter',
        [k('PAUSE')] = 'pause',
        [k('ESCAPE')] = 'esc',
        [k('SPACE')] = 'space',
        [k('EXCLAIM')] = '!',
        [k('QUOTEDBL')] = '"',
        [k('HASH')] = '#',
        [k('DOLLAR')] = '$',
        [k('AMPERSAND')] = '&',
        [k('QUOTE')] = '\'',
        [k('LEFTPAREN')] = '(',
        [k('RIGHTPAREN')] = ')',
        [k('ASTERISK')] = '*',
        [k('PLUS')] = '+',
        [k('COMMA')] = ',',
        [k('MINUS')] = '-',
        [k('PERIOD')] = '.',
        [k('SLASH')] = '/',
        [k('0')] = '0',
        [k('1')] = '1',
        [k('2')] = '2',
        [k('3')] = '3',
        [k('4')] = '4',
        [k('5')] = '5',
        [k('6')] = '6',
        [k('7')] = '7',
        [k('8')] = '8',
        [k('9')] = '9',
        [k('COLON')] = ':',
        [k('SEMICOLON')] = ';',
        [k('LESS')] = '<',
        [k('EQUALS')] = '=',
        [k('GREATER')] = '>',
        [k('QUESTION')] = '?',
        [k('AT')] = '@',
        [k('LEFTBRACKET')] = '[',
        [k('BACKSLASH')] = '\\',
        [k('RIGHTBRACKET')] = ']',
        [k('CARET')] = '^',
        [k('UNDERSCORE')] = '_',
        [k('BACKQUOTE')] = '`',
        [k('a')] = 'a',
        [k('b')] = 'b',
        [k('c')] = 'c',
        [k('d')] = 'd',
        [k('e')] = 'e',
        [k('f')] = 'f',
        [k('g')] = 'g',
        [k('h')] = 'h',
        [k('i')] = 'i',
        [k('j')] = 'j',
        [k('k')] = 'k',
        [k('l')] = 'l',
        [k('m')] = 'm',
        [k('n')] = 'n',
        [k('o')] = 'o',
        [k('p')] = 'p',
        [k('q')] = 'q',
        [k('r')] = 'r',
        [k('s')] = 's',
        [k('t')] = 't',
        [k('u')] = 'u',
        [k('v')] = 'v',
        [k('w')] = 'w',
        [k('x')] = 'x',
        [k('y')] = 'y',
        [k('z')] = 'z',
        [k('DELETE')] = 'del',
        [k('KP0')] = 'np0',
        [k('KP1')] = 'np1',
        [k('KP2')] = 'np2',
        [k('KP3')] = 'np3',
        [k('KP4')] = 'np4',
        [k('KP5')] = 'np5',
        [k('KP6')] = 'np6',
        [k('KP7')] = 'np7',
        [k('KP8')] = 'np8',
        [k('KP9')] = 'np9',
        [k('KP_PERIOD')] = 'np.',
        [k('KP_DIVIDE')] = 'np/',
        [k('KP_MULTIPLY')] = 'np*',
        [k('KP_MINUS')] = 'np-',
        [k('KP_PLUS')] = 'np+',
        [k('KP_ENTER')] = 'npenter',
        [k('KP_EQUALS')] = 'np=',
        [k('UP')] = 'up',
        [k('DOWN')] = 'down',
        [k('RIGHT')] = 'right',
        [k('LEFT')] = 'left',
        [k('INSERT')] = 'ins',
        [k('HOME')] = 'home',
        [k('END')] = 'end',
        [k('PAGEUP')] = 'pgup',
        [k('PAGEDOWN')] = 'pgdn',
        [k('F1')] = 'f1',
        [k('F2')] = 'f2',
        [k('F3')] = 'f3',
        [k('F4')] = 'f4',
        [k('F5')] = 'f5',
        [k('F6')] = 'f6',
        [k('F7')] = 'f7',
        [k('F8')] = 'f8',
        [k('F9')] = 'f9',
        [k('F10')] = 'f10',
        [k('F11')] = 'f11',
        [k('F12')] = 'f12',
        [k('F13')] = 'f13',
        [k('F14')] = 'f14',
        [k('F15')] = 'f15',
        [k('NUMLOCK')] = 'numlock',
        [k('CAPSLOCK')] = 'capslock',
        [k('SCROLLOCK')] = 'scrolllock',
        [k('RSHIFT')] = 'rshift',
        [k('LSHIFT')] = 'lshift',
        [k('RCTRL')] = 'rctrl',
        [k('LCTRL')] = 'lctrl',
        [k('RALT')] = 'ralt',
        [k('LALT')] = 'lalt',
        [k('RMETA')] = 'rmeta',
        [k('LMETA')] = 'lmeta',
        [k('LSUPER')] = 'lsuper',
        [k('RSUPER')] = 'rsuper',
        [k('MODE')] = 'mode',
        [k('HELP')] = 'help',
        [k('PRINT')] = 'print',
        [k('SYSREQ')] = 'sysrq',
        [k('BREAK')] = 'break',
        [k('MENU')] = 'menu',
        [k('POWER')] = 'power',
        [k('EURO')] = 'euro',
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
