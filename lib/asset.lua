-- Quipkit asset framework.
-- This lets you write scripts that load and add methods to your game assets.

module(..., package.seeall)



-- Directory where assets are stored.
location = "gamedata/assets"

-- Load an asset script by calling this package with the asset's name.
do
    local fenv = getfenv()
    local fenv_mt = getmetatable(fenv)
    if not fenv_mt then
        fenv_mt = {}
        setmetatable(fenv, fenv_mt)
    end
    fenv_mt.__call = function (tbl, asset_name)
        return require(location .. "/" .. asset_name)
    end
end
