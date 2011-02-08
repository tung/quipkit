local arg = { n = select('#', ...); ... }

print("Hello, World! " .. tostring(arg.n) .. " argument(s) passed" .. (arg.n == 0 and "." or ":"))
for _, v in ipairs(arg) do
    print(v)
end
