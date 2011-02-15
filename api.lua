--[[ See copyright notice in doc/COPYRIGHT. ]]

-- Add path of library bootstrapping scripts to package.path.
local old_package_path = package.path
package.path = './lib/?.lua;' .. package.path

-- Detect Windows.
rawset(_G, "WIN32", os.getenv("WINDIR") ~= nil)

-- Pre-load engine API.
require 'game'
require 'gfx'

package.path = old_package_path
