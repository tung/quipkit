-- Quipkit gamelet framework.
-- Gamelets are pieces of a complete game that can update and draw themselves.
-- Gamelets can be run by pushing instances of them onto the gamelet stack.

require "game"

module(..., package.seeall)



-- Holds loaded gamelets, i.e. tables loaded with gamelet methods.
loaded = {}

-- Holds gamelet instances to be run.
stack = {
    push = function (self, gamelet_instance)
        self[#self + 1] = gamelet_instance
    end,
    pop = function (self)
        if #self < 1 then
            error("Can't pop from empty gamelet stack", 2)
        end
        local gamelet_instance = self[#self]
        self[#self] = nil
        return gamelet_instance
    end
}



-- Retrieve a gamelet (table) by calling this package like a function.
do
    fenv_mt = getmetatable(getfenv())
    if not fenv_mt then
        fenv_mt = {}
        setmetatable(getfenv(), fenv_mt)
    end
    fenv_mt.__call = function (self, gamelet_name)
        if not loaded[gamelet_name] then
            local code = loadfile("gamedata/gamelets/" .. gamelet_name .. ".lua")
            if not code then
                error("Could not load gamelet '" .. gamelet_name .. "'", 2)
            end

            -- Should fill loaded[gamelet_name] via calling gamelet.new(...).
            code(gamelet_name)
        end
        return loaded[gamelet_name]
    end
end


-- Prepare an environment table for the gamelet calling this function.
-- A call to this is typically one of the first lines in a gamelet.
function new(gamelet_name)
    local gamelet = {}
    gamelet.GAMELET_NAME = gamelet_name
    gamelet.GAMELET_INST_MT = { __index = gamelet }
    setmetatable(gamelet, {__index = getfenv(2)})
    loaded[gamelet_name] = gamelet
    setfenv(2, gamelet)
end


-- Create an instance of the gamelet in which this function is called.
-- Typically called within the gamelet's init function.
function instance(gamelet)
    local inst = {}
    setmetatable(inst, getfenv(2).GAMELET_INST_MT)
    return inst
end


-- Initialization hook function. Set by the user to prepare the stack
-- before running.
init = function () end

-- Valid return values for gamelet hooks.
GAMELET_QUIT = 1        -- End the gamelet game loop.
GAMELET_REDRAW = 2      -- Call draw() hook of the top-most gamelet.
GAMELET_NEXT = 3        -- Call the gamelet hook below this one.

-- Gamelet event handling hook.
function game.event(e)
    for i = #stack, 1, -1 do
        local event_fn = stack[i].event
        if event_fn then
            local e_return = event_fn(stack[i], e)
            if e_return == GAMELET_REDRAW then
                return game.GAME_REDRAW
            elseif e_return == GAMELET_QUIT then
                return game.GAME_QUIT
            elseif e_return ~= GAMELET_NEXT then
                -- Stop if GAMELET_NEXT is _not_ returned.
                return
            end
        end
    end
end

-- Gamelet update hook.
function game.update(delta)
    for i = #stack, 1, -1 do
        local update_fn = stack[i].update
        if update_fn then
            local u_return = update_fn(stack[i], delta)
            if u_return == GAMELET_REDRAW then
                return game.GAME_REDRAW
            elseif u_return == GAMELET_QUIT then
                return game.GAME_QUIT
            elseif e_return ~= GAMELET_NEXT then
                -- Stop if GAMELET_NEXT is _not_ returned.
                return
            end
        end
    end

    -- If the stack is empty, stop the game loop.
    return game.GAME_QUIT
end

-- Gamelet draw hook.
function game.draw()
    game.clearScreen()

    for i = #stack, 1, -1 do
        local draw_fn = stack[i].draw
        if draw_fn then
            if draw_fn(stack[i]) ~= GAMELET_NEXT then
                break
            end
        end
    end
end

-- Start running gamelets on the gamelet stack.
function run()
    game.init = init
    game.run()
end
