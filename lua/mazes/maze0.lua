--[[
maze0.lua: a simple maze algorithm based on program#1 on pg56 of
    https://archive.org/details/1981-12-compute-magazine/page/n57/mode/2up?view=theater
    The program listing is called MAZE GENERATOR(1980) BY C. BOND

date: 26/02/2022
author: Abhishek Mishra

© 2022 Abhishek Mishra <abhishekmishra.in>
--]]--
local WALL = '█'
local NOWALL = ' '
local EMPTYCELL = '-'
local CURRENTCELL = '▲'
local MARKEDCELL = '◉'
local VISITEDCELL = '★'
local BEGIN = '◊'

local DIRECTIONS = {'N', 'E', 'S', 'W'}

function createField(width, height)
    local field = {}
    for i = 1, (2 * height) + 1, 1 do
        field[i] = {}
        for j = 1, (2 * width) + 1, 1 do
            field[i][j] = WALL
            if i % 2 == 0 and j % 2 == 0 then
                field[i][j] = EMPTYCELL
            end
        end
    end
    field.width = width
    field.height = height
    field.pos = locn(2, 2)
    return field
end

function printField(field)
    if field == nil then
        error('field is nil')
    end

    print(
        string.format(
            '==== MAZE [%02d, %02d] =====', field.width, field.height))

    for i = 1, (2 * field.height) + 1, 1 do
        io.write(' ')
        for j = 1, (2 * field.width) + 1, 1 do
            io.write(field[i][j] .. ' ')
        end
        print()
    end

    print('=== ===== MAZE ===== ===')
end

function printLoc(loc, name)
    if loc == nil then
        error('loc is nil')
    end

    local prefix = ''
    if name ~= nil then
        prefix = name .. ' -> '
    end

    print(string.format('%s[%d, %d]', prefix, loc.x, loc.y))
end

function getVal(field, p)
    return field[p.y][p.x]
end

function setVal(field, p, val)
    field[p.y][p.x] = val
end

function getRandomDirection()
    return DIRECTIONS[math.random(1, 4)]
end

function locn(x, y)
    local x = x or 1
    local y = y or 1
    return {
        x = x,
        y = y
    }
end

function validDirection(field, direction)
    if field == nil then
        error('field is nil')
    end
    local pos = field.pos
    local valid = true
    local marker = false
    if direction == 'N' and pos.y == 2 then
        valid = false
    elseif direction == 'E' and pos.x == (2 * field.width) then
        valid = false
    elseif direction == 'W' and pos.x == 2 then
        valid = false
    elseif direction == 'S' and pos.y == (2 * field.height) then
        valid = false
    end

    if valid then
        -- printLoc(pos, 'dir move ' .. direction .. ' is valid')
        local locInDir = locInDirection(field, direction)
        marker = hasMarker(field, locInDir)
        -- printLoc(
        --     locInDir, 'locInDir is valid ' .. tostring(valid) .. ' marker = ' ..
        --         tostring(marker))
    end

    return valid, marker
end

function getWallInDirection(field, direction)
    if field == nil then
        error('field is nil')
    end
    local pos = field.pos

    local wallPos = nil
    if direction == 'N' then
        wallPos = locn(pos.x, pos.y - 1)
    elseif direction == 'E' then
        wallPos = locn(pos.x + 1, pos.y)
    elseif direction == 'W' then
        wallPos = locn(pos.x - 1, pos.y)
    elseif direction == 'S' then
        wallPos = locn(pos.x, pos.y + 1)
    else
        error('unknown direction ' .. direction)
    end
    return wallPos
end

function rotateDirection(direction)
    if direction == 'N' then
        return 'E'
    elseif direction == 'E' then
        return 'S'
    elseif direction == 'S' then
        return 'W'
    elseif direction == 'W' then
        return 'N'
    else
        error('unknown direction ' .. direction)
    end
end

function hasMarker(field, loc)
    if field == nil then
        error('field is nil')
    end

    return not (getVal(field, loc) == EMPTYCELL)
end

function oob(field, loc)
    if loc.x < 2 or loc.x > (2 * field.width) or loc.y < 2 or loc.y >
        (2 * field.height) then
        return true
    end
    return false
end

function locInDirection(field, direction)
    if field == nil then
        error('field is nil')
    end
    local pos = field.pos

    if direction == 'N' then
        pos = locn(pos.x, pos.y - 2)
    elseif direction == 'E' then
        pos = locn(pos.x + 2, pos.y)
    elseif direction == 'S' then
        pos = locn(pos.x, pos.y + 2)
    elseif direction == 'W' then
        pos = locn(pos.x - 2, pos.y)
    else
        error('unknown direction ' .. direction)
    end

    return pos
end

function move(field, direction)
    local pos = locInDirection(field, direction)
    field.pos = pos
end

function moveRemoveWall(field, direction)
    local wallToB = getWallInDirection(field, direction)
    -- printLoc(wallToB, 'wallToB')
    setVal(field, wallToB, NOWALL)
    if getVal(field, field.pos) ~= BEGIN then
        setVal(field, field.pos, MARKEDCELL)
    end
    move(field, direction)
    setVal(field, field.pos, CURRENTCELL)
    -- printLoc(field.pos, 'fd.pos')
end

function printPeekDirs(pd)
    for k, _ in pairs(pd) do
        io.write(k .. ',')
    end
    print()
end

function allPeekDirs(pd)
    if pd['N'] and pd['E'] and pd['W'] and pd['S'] then
        return true
    end
    return false
end

function backupPos(fd)
    for _, dir in ipairs({'N', 'E', 'W', 'S'}) do
        local p = locInDirection(fd, dir)
        local w = getVal(fd, getWallInDirection(fd, dir))
        -- printLoc(p, 'loc in dir '.. dir .. ' and wall ' .. w .. ' and oob ' .. tostring(oob(fd, p)))
        if (not oob(fd, p)) and (getVal(fd, p) == MARKEDCELL) and (w == NOWALL) then
            -- printLoc(p, 'found backup pos')
            return p
        else
            -- printLoc(p, 'is not backup pos')
        end
    end
    return nil
end

function createMaze(width, height, seed)
    local fd = createField(width, height)
    fd.seed = seed or os.time()
    math.randomseed(fd.seed)

    -- set starting position as BEGIN
    setVal(fd, fd.pos, BEGIN)
    -- printField(fd)

    -- set previous position to nil
    local prevPos = nil
    local backedUp = false
    local done = false
    local tried = false
    local direction = nil
    local valid = false
    local step = nil

    local peekDirs = {}

    -- get a random direction and check if it is valid
    local direction = getRandomDirection()
    local valid, tried = validDirection(fd, direction)
    peekDirs[direction] = true

    -- repeat
    while true do
        -- printPeekDirs(peekDirs)
        -- printLoc(fd.pos, 'current')
        -- print(string.format('direction = %s is valid = %s', direction, valid))
        step = nil
        -- check if direction is valid
        if valid and (not tried) then
            -- if yes remove wall and move ahead in the direction
            prevPos = fd.pos
            moveRemoveWall(fd, direction)
            peekDirs = {}
            step = 'moved'
        else
            -- else rotate to next direction
            direction = rotateDirection(direction)
            peekDirs[direction] = true

            -- check if this is a tried direction
            valid, tried = validDirection(fd, direction)
            -- if valid then
            --     printLoc(locInDirection(fd, direction), 'peek ' .. direction)
            --     tried = hasMarker(fd, locInDirection(fd, direction))
            -- end

            local allTried = allPeekDirs(peekDirs)

            if valid and tried then
                if not allTried then
                    step = 'rotate'
                else
                    -- if home point then end
                    -- else erase marker and back up
                    -- printLoc(locInDirection(fd, direction), 'tried')
                    local marker = getVal(fd, fd.pos)
                    setVal(fd, fd.pos, VISITEDCELL)
                    if (marker == BEGIN) then
                        done = true
                        step = 'done'
                    else
                        local bpos = backupPos(fd)
                        if bpos ~= nil then
                            fd.pos = bpos
                            setVal(fd, fd.pos, CURRENTCELL)
                            backedUp = true
                            -- print('backing up')
                            step = 'backedup'
                            peekDirs = {}
                        else
                            -- print('backupPos is nil, nowhere to go, we\'re done.')
                            done = true
                            step = 'done'
                        end
                    end
                end
            end

            if (not valid) or (not tried) then
                step = 'rotate'
            end

            if valid and (not tried) then
                step = 'move'
            end
        end

        -- print('step = ' .. step)
        if step == 'rotate' or step == 'move' then
            -- print(step .. ' -- move or rotate on to next cell')
        elseif step == 'done' then
            break
        elseif step == 'moved' or step == 'backedup' then
            -- get a random direction and check if it is valid
            direction = getRandomDirection()
            peekDirs[direction] = true
            valid, tried = validDirection(fd, direction)
        else
            error('unknown step')
        end

        -- printField(fd)
        -- io.read()
    end
    return fd
end

local fd, mazeseed = createMaze(20, 20)
printField(fd)

function turtlePrintField(field)
    if field == nil then
        error('field is nil')
    end

    local default_pencolour = function()
        pencolour(0, 0, 0)
    end

    local startcell_pencolour = function()
        pencolour(255, 0, 0)
    end

    local endcell_pencolour = function()
        pencolour(0, 255, 0)
    end

    local cellwidth = 50
    local cellheight = 50

    local mazewidth = (2 * field.width) + 1
    local mazeheight = (2 * field.height) + 1

    -- size has margin rows and columns
    canvas_size(cellwidth * (mazewidth + 2), cellheight * (mazeheight + 4))

    penup()

    default_pencolour()

    penwidth(cellheight)
    heading(0.0)

    font('Calibri', cellheight)
    setpos(cellwidth, getheight() - (cellheight * 1.5))
    pendown()
    filltext(
        string.format(
            'MAZE [%02d, %02d] - #%d', mazewidth, mazeheight, field.seed))

    penup()
    setpos(cellwidth, getheight() - (cellheight * 2.5))
    pendown()

    penwidth(cellheight)
    heading(0.0)

    for i = 1, mazeheight, 1 do
        for j = 1, mazewidth, 1 do
            -- io.write(field[i][j] .. ' ')
            local cell = field[i][j]
            if i == 2 and j == 2 then
                startcell_pencolour()
                forward(cellwidth)
                default_pencolour()
            elseif i == (mazeheight - 1) and j == (mazewidth - 1) then
                endcell_pencolour()
                forward(cellwidth)
                default_pencolour()
            elseif cell == WALL then
                forward(cellwidth)
            else
                penup()
                forward(cellwidth)
                pendown()
            end
            -- penup()
            -- forward(cellwidth)
            -- pendown()
        end
        penup()
        setpos(cellwidth, getheight() - (cellheight * (i + 2.5)))
        pendown()
    end

    penup()
    setpos(cellwidth, cellheight * 0.8)
    pendown()
    font('Calibri', cellheight * 0.8)
    filltext(
        string.format(
            'generated on %s, © 2022 Abhishek Mishra',
            os.date('%d/%m/%Y %H:%M:%S')))
end

if _turtle then
    turtlePrintField(fd)
    export_img('out/maze0.png')
end
