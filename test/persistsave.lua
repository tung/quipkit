require "persist"

a = {x = 1, y = 2, z = {3, 4}}

metatable_for_a = {
    speak = function (self)
        print("I'm A.", self.x, self.y)
    end
}
metatable_for_a.__index = metatable_for_a

metatable_for_z = {
    speak = function (self)
        print("I'm Z.", self[1], self[2])
    end
}
metatable_for_z.__index = metatable_for_z

setmetatable(a, metatable_for_a)
setmetatable(a.z, metatable_for_z)

persist.saveToFile(a, "test/persist.tbl")
a:speak()
a.z:speak()
