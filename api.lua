--[[ See copyright notice in doc/COPYRIGHT. ]]

-- Add path of library bootstrapping scripts to package.path.
local old_package_path = package.path
local package_path_list = {}
for part in string.gmatch(package.path, "[^%;]+") do
    table.insert(package_path_list, part)
end
table.insert(package_path_list, 1, "./lib/?.lua")
package.path = table.concat(package_path_list, ";")

-- Detect Windows.
rawset(_G, "WIN32", os.getenv("WINDIR") ~= nil)

-- Pre-load modules for wrap.lua.
require 'gl'
require 'sdl'

-- Pre-load engine API.
require 'game'

package.path = old_package_path
