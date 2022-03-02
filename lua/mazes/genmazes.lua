local maze = require 'mazes/maze'

local numMazes = 1
if #arg > 0 then
    numMazes = arg[1]
end

print ('Creating ' .. numMazes .. ' mazes...')

-- create numMazes mazes
for i = 1, numMazes, 1 do
    local fd = maze.createMaze(math.random(10, 30), math.random(10, 30))
    -- maze.printField(fd)
    local outfile = string.format('out/maze-%dx%d-%d.png', fd.mazewidth, fd.mazeheight, fd.seed)
    maze.turtlePrintField(fd, outfile)
end
