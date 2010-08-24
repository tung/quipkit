require "asset"

print("Press ENTER/RETURN after each line.")
print("")

local test_textflow = asset("textflow"):new("test1")
while test_textflow:more() do
    print(test_textflow:line())
    io.read()
    test_textflow:forward()
end
