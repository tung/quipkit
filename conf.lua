--[[ See copyright notice in doc/COPYRIGHT. ]]

-- Config loader: tries to load the game config file based on the --config or --script command line arguments.
local function UpToLastChar(str, char)
    local result = ""
    local found = 0
    local i = 1
    for ch in string.gmatch(str, '.') do
        if ch == char then
            found = i
        end
        i = i + 1
    end
    if found > 0 then
        result = string.sub(str, 1, found)
    end
    return result
end


local function DirName(path)
    return UpToLastChar(path, string.sub(package.config, 1, 1))
end


local function LoadConfig(conf_path)
    local conf = {}
    local chunk = loadfile(conf_path)
    if chunk == nil then
        return nil
    else
        setfenv(chunk, conf)
        chunk()
    end
    if conf.base then
        conf.base = DirName(conf_path) .. conf.base
    end
    return conf
end



local args = ...
local conf = nil

if args.config then
    conf = LoadConfig(args.config)
    if conf == nil then
        error("couldn't load config file " .. args.config)
    end
else
    local script = args.script
    local specific_path = string.sub(UpToLastChar(script, "."), 1, -2) .. "-config.txt"
    local general_path = DirName(script) .. "config.txt"
    conf = LoadConfig(specific_path)
    if conf == nil then
        conf = LoadConfig(general_path)
    end
end

return conf
