-- persist.lua
-- Save tables in a way that lets them restore their metatables when loaded.

module(..., package.seeall)



-- Get a reverse lookup table whose keys are tables, and values are arrays
-- of strings that, when keyed successively from _G at the level, refers
-- to that table.
function getTablePaths(level)
    level = level or 1
    local paths = {}
    local queue = {{getfenv(level + 1), {}}}
    local i = 1
    while queue[i] do
        local current_table = queue[i][1]
        local current_path = queue[i][2]
        for k, v in pairs(current_table) do
            if type(v) == "table" and not paths[v] then
                local next_path = {}
                for j = 1, #current_path do
                    next_path[j] = current_path[j]
                end
                next_path[#next_path + 1] = k
                paths[v] = next_path
                queue[#queue + 1] = {v, next_path}
            end
        end
        i = i + 1
    end
    return paths
end


-- Copy a table. Only copy number, string and table keys and values.
-- Create proxies for tables so that if it has a metatable, it can
-- be restored at load time.
function proxyCopy(tbl, fenv_level)
    local table_paths = getTablePaths(fenv_level + 1)
    local processed = {}
    local function processTable(t)
        if not processed[t] then
            processed[t] = {tbl = {}}

            for k, v in pairs(t) do
                local k_type = type(k)
                local v_type = type(v)
                if (k_type == "number" or k_type == "string" or k_type == "table") and
                        (v_type == "number" or v_type == "string" or v_type == "table") then
                    local new_k
                    if k_type == "table" then
                        new_k = processTable(k)
                    else
                        new_k = k
                    end

                    local new_v
                    if v_type == "table" then
                        new_v = processTable(v)
                    else
                        new_v = v
                    end

                    processed[t].tbl[new_k] = new_v
                end
            end

            processed[t].mt = table_paths[getmetatable(t)]
        end
        return processed[t]
    end
    return processTable(tbl)
end


-- Copy a table, restoring proxy tables and metatables.
function unproxyCopy(tbl, fenv_level)
    local caller_fenv = getfenv(fenv_level + 1)
    local processed = {}
    local function processTable(t)
        if not processed[t] then
            processed[t] = {}

            for k, v in pairs(t.tbl) do
                local new_k
                if type(k) == "table" then
                    new_k = processTable(k)
                else
                    new_k = k
                end

                local new_v
                if type(v) == "table" then
                    new_v = processTable(v)
                else
                    new_v = v
                end

                processed[t][new_k] = new_v
            end

            if t.mt then
                local mt = caller_fenv
                for i = 1, #(t.mt) do
                    mt = mt[t.mt[i]]
                    if not mt then
                        local err_str = "Could not resolve " .. t.mt[1]
                        for e = 2, i do
                            err_str = err_str .. "." .. t.mt[e]
                        end
                        error(err_str)
                    end
                end
                setmetatable(processed[t], mt)
            end
        end
        return processed[t]
    end
    return processTable(tbl)
end


-- Save a table to a string with env as data to be passed to a pre-load hook
-- when loading the table data again.
-- Returns the string on success, or nil on failure.
function saveToString(tbl, env)
    local to_save = {
        data = proxyCopy(tbl, 2),
        env = env
    }
    return table.save(to_save)
end


-- Save a table to a string using a buffer (for larger tables), with env
-- as data to be passed to a pre-load hook when loading the table data again.
-- Returns the string on success, or nil on failure.
function saveToBufferedString(tbl, env)
    local to_save = {
        data = proxyCopy(tbl, 2),
        env = env
    }
    return table.save(to_save, true)
end


-- Save a table to the given file, with env as data to be passed to a
-- pre-load hook when loading the table data again.
-- Returns 1 on success, or nil and an error message on failure.
function saveToFile(tbl, filename, env)
    local to_save = {
        data = proxyCopy(tbl, 2),
        env = env
    }
    return table.save(to_save, filename)
end


-- Load a table saved as a string or as a file. restore_env_hook is called
-- with the env passed when the table was saved, to restore environment,
-- e.g. loaded Lua packages.
-- Returns the restored table on success, or nil and an error message
-- on failure.
function load(string_or_filename, restore_env_hook)
    local freshly_loaded, error_message = table.load(string_or_filename)
    if not freshly_loaded then
        return nil, error_message
    end
    if restore_env_hook then
        restore_env_hook(freshly_loaded.env)
    end
    return unproxyCopy(freshly_loaded.data, 2)
end
