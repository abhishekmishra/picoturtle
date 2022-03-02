--[[
runSamples.lua: execute all items in the turtle samples
                and export images from the result

date: 25/02/2022
author: Abhishek Mishra
--]]

local samplesDir = "lua/samples/"
local sampleOutDir = "out/"

local samplesList = {
    "empty",
    "fern",
    "polygons",
    "sierpinski",
    "spiral",
    "text-demo",
    "tree",
    "turtle_basic"
}

for _, sample in ipairs(samplesList) do
    if sample ~= nil then
        local sampleFile = samplesDir .. sample .. '.lua'
        local sampleOutFile = sampleOutDir .. sample .. '.png'
        print('==== Run '.. sampleFile .. ' and export ' .. sampleOutFile .. " ====")

        clear()
        reset()

        dofile(sampleFile)

        stop()
        export_img(sampleOutFile)
    end
end