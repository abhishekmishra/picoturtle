# PicoTurtle
- A simple Turtle programming environment.
- Turtle programs are written in Lua in cross-platform GUI.
## Status
Currently an early dev only alpla release. Expect many changes.

## Build
- Uses cmake and vcpkg for build. See the CMakeLists.txt file for details.
- Use the Makefile target `make genbuild build` to generate a cmake build and then call the build for your platform.
- Detailed build instructions will be provided in subsequent releases.

## Run
- Use make target `make run` to run the program.
- Editor will load with a sample program.
- Look under <project_dir>/lua for sample programs which will run with PicoTurtle.