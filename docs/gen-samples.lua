--[[
gen-samples.lua: generate a listing of each sample given in the samples list.

author: Abhishek Mishra
date  : 20/01/2022
--]]

package.path = package.path .. "."

local etlua = require "etlua"

--- template for the sample program listing
local sample_template = etlua.compile([[
# <%- turtle_sample_name %>

## Program

```lua
<%- turtle_code %>
```
## Output Image

![Turtle Output](<%- turtle_img %>)

]])

--- sample turtle programs folder
local samples_folder = "../lua/samples/"

--- list of sample programs
local samples_list = {
	"fern",
	"polygons",
	"sierpinski",
	"spiral",
	"text-demo",
	"tree",
	"turtle_basic"
}

--- output folder for the markdown dows
local docs_folder = "docs/"

--- output folder for the turtle output images
local images_folder = "docs/images/"

local image_path_in_md = "images/"

--- picoturtle cli app
local ptcli_path = "\"..\\build\\bin\\Debug\\ptcli\""

for k, sample in ipairs(samples_list) do
	local prog = "\"" .. ptcli_path .. " \"-o\" \"" .. images_folder ..
		"sample_" .. sample .. ".png\" \"" ..
		samples_folder .. sample .. ".lua\"\""

	print("Running |" .. prog .. "|")
	os.execute(prog)
end

for k, sample in ipairs(samples_list) do
	local turtle_img = image_path_in_md .. "sample_" .. sample .. ".png"
	local turtle_filename = samples_folder .. sample .. ".lua"
	local turtle_file = io.open(turtle_filename, "r")
	if (turtle_file) then
		local turtle_code = turtle_file:read("*all")
		turtle_file:close()

		local sample_args = {}
		sample_args["turtle_sample_name"] = sample
		sample_args["turtle_code"] = turtle_code
		sample_args["turtle_img"] = turtle_img
		local hppoutput = sample_template(sample_args)

		print(hppoutput)

		local turtle_md_file = io.open(docs_folder .. "sample_" .. sample .. ".md", "w")
		if (turtle_md_file) then
			turtle_md_file:write(hppoutput)
			turtle_md_file:close()
		end
	end
end
