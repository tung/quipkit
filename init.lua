-- Load support libraries from lib/
package.path = "./lib/?.lua;" .. package.path

-- Detect Windows
rawset(_G, "WIN32", os.getenv("WINDIR") ~= nil)

-- Provide table.save and table.load functions
dofile("./lib/tablesave.lua")
