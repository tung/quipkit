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


-- Save a table.
-- If mode is "string", return a string representation of the table.
-- If mode is "tmpfile", return a string made from a temporary file buffer.
-- If mode is "file", save the table to the given filename.
function save(tbl, mode, filename)
end


function load(src)
end
