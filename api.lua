-- Add path of library bootstrapping scripts to package.path.
local old_package_path = package.path
local package_path_list = {}
for part in string.gmatch(package.path, "[^%;]+") do
    table.insert(package_path_list, part)
end
table.insert(package_path_list, 1, "./lib/?.lua")
package.path = table.concat(package_path_list, ";")

-- TODO: Load Quipkit API here.

package.path = old_package_path
