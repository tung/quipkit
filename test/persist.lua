require "persist"



local printValue, printTable



function printValue(v, indent)
    local s = ""
    if type(v) == "number" then
        s = tostring(v)
    elseif type(v) == "string" then
        s = string.format("%q", v)
        s = string.gsub(s, "\\\n", "\n")
        s = string.gsub(s, "\\\r", "\r")
        s = string.gsub(s, string.char(26), "\\026")
    elseif type(v) == "table" then
        s = printTable(v, indent)
    end
    return s
end


function printTable(t, indent)
    indent = indent or 0
    local s = ""

    s = s .. "{\n"
    for k, v in pairs(t) do
        for i = 1, indent + 1 do s = s .. "  " end
        s = s .. "["
        s = s .. printValue(k)
        s = s .. "] = "
        s = s .. printValue(v, indent + 1)
        s = s .. ",\n"
    end
    for i = 1, indent do s = s .. "  " end
    s = s .. "}"
    return s
end



a = {x = 1, y = "two", z = {3, "four"}; 5, "six", 7}
print(printTable(persist.work(a)))
