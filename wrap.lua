--[[ See copyright notice in doc/COPYRIGHT ]]

local opts = ...


local function init()
    print("Init:")
    print("width = ", opts.width)
    print("height = ", opts.height)
    print("fullscreen = ", opts.fullscreen and "true" or "false")
    print("channels = ", opts.channels)
    print("--")
end


local function close()
    print("--")
    print("Closing...")
end


return init, close
