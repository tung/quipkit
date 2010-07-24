-- persist.lua
-- Save tables in a way that lets them restore their metatables when loaded.



module(..., package.seeall)



function work(t)
    -- TODO: Rework table so data needed to restore metatables are preserved.
    return t
end


function unwork(src)
    -- TODO
    return {}
end
