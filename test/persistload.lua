require "persist"

metatable_for_a = {
    speak = function (self)
        print("I'm a newer version of A.", self.x, self.y)
    end
}
metatable_for_a.__index = metatable_for_a

metatable_for_z = {
    speak = function (self)
        print("I'm a newer version of Z.", self[1], self[2])
    end
}
metatable_for_z.__index = metatable_for_z

a = persist.load("test/persist.tbl")
a:speak()
a.z:speak()
