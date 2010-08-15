-- Quipkit gamelet framework.
-- Gamelets are pieces of a complete game that can update and draw themselves.
-- Gamelets can be run by pushing instances of them onto the gamelet stack.

module(..., package.seeall)



-- Directory where gamelets are stored.
location = "gamedata/gamelets"

-- Load a gamelet by calling this package with the gamelet's name.
do
    local fenv = getfenv()
    local fenv_mt = getmetatable(fenv)
    if not fenv_mt then
        fenv_mt = {}
        setmetatable(fenv, fenv_mt)
    end
    fenv_mt.__call = function (tbl, gamelet_name)
        return require(location .. "/" .. gamelet_name)
    end
end

-- Gamelet stack. Use Lua table functions like table.insert and
-- table.remove to control your game's logic.
stack = {}

-- Return gamelet.NEXT from hooks to call the same hook
-- in the gamelet below this one in the stack.
NEXT = {}

local function call_hook(self, pos, hook_name, ...)
    local gamelet_inst = self[pos]
    if gamelet_inst then
        local hook = gamelet_inst[hook_name]
        if hook then
            local hook_args = {gamelet_inst, ...}
            table.insert(hook_args, pos)
            local ret_val = hook(unpack(hook_args))
            if ret_val == NEXT and pos > 1 then
                return call_hook(self, pos - 1, hook_name, ...)
            end
            return ret_val
        end
    end
end

-- gamelet.stack:event(e) calls the event hook of the top-most gamelet.
-- gamelet.stack:update(delta) calls the update hook of the top-most gamelet.
-- gamelet.stack:draw() calls the draw hook of the top-most gamelet.
-- Return gamelet.NEXT from a hook to go down one further.
stack_mt = {
    __index = {
        event = function (self, e)
            return call_hook(self, #self, 'event', e)
        end,
        update = function (self, delta)
            return call_hook(self, #self, 'update', delta)
        end,
        draw = function (self)
            return call_hook(self, #self, 'draw')
        end
    }
}

setmetatable(stack, stack_mt)
