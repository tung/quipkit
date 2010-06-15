-- Load support libraries from lib/
package.path = "./dev/lib/?.lua;" .. package.path

-- Detect Windows
rawset(_G, "WIN32", os.getenv("WINDIR") ~= nil)
