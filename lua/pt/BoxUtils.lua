--- Utility functions for boxes (might move to their own types/modules later)
--
local Vec2 = require'pt/Vec2'
local Box = require'pt/Box'

--- Create a grid of boxes (nrows x ncols) with size such that they fit in the given parent Box
-- @tparam Box parent
-- @tparam number nrows
-- @tparam number ncols
local function create_box_grid(parent, nrows, ncols)
	if parent == nil then
		error("parent box is nil!")
	end

	local nrows = nrows or 1
	local ncols = ncols or 1

	local dim = parent:canvas_dim()
	local orig = parent:canvas_orig()

	local cdim = Vec2:new(dim:w()/ncols, dim:h()/nrows)
	-- print(cdim:w() .. 'x' .. cdim:h())

	local grid = {}
	for i = 0, ncols-1 do
		for j = 0, nrows-1 do
			local orig = Vec2:new(i * (dim:w()/ncols), j * (dim:h()/nrows))
			local d = Vec2:new(cdim:w(), cdim:h())
			-- print(i .. ',' .. 'j = ' .. orig:x() .. 'x' .. orig:y())
			table.insert(grid, Box:new(orig, d))
		end
	end
	return grid
end

--- @export
return {
	create_box_grid = create_box_grid
}