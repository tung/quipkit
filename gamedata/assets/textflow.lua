require "asset"

module(..., package.seeall)

MT = { __index = getfenv() }

function new(ast, id)
    local a = {
        index = 1,
        lines = {}
    }

    local path = table.concat({asset.location, "textflow", id}, "/") .. ".txt"
    for line in io.lines(path) do
        table.insert(a.lines, line)
    end

    setmetatable(a, MT)
    return a
end

function line(self)
    return self.lines[self.index]
end

function more(self)
    return self.index <= #self.lines
end

function forward(self)
    self.index = self.index + 1
end
