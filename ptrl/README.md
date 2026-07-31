# PicoTurtle 0.3.0 experiment

This directory contains the Raylib-based PicoTurtle runtime being developed for
the `0.3.0` release, codenamed `matrix-goose`. It runs Lua turtle programs
without the original Qt IDE or Skia renderer.

## Build

The build uses CMake, vcpkg, and the Makefile in this directory:

```sh
make genbuild
make build
```

## Run

Pass one Lua program to the executable:

```sh
./build/ptrl ../lua/samples/polygons.lua
```

The normal windowed mode creates a persistent Raylib canvas, executes the Lua
program, displays the completed canvas, and keeps the window responsive until
the user closes it or presses Escape.

Canvas updates are visible after each drawing operation by default.
`disable_update()` keeps drawing into the off-screen canvas without presenting
it, `paint()` presents it explicitly, and `enable_update()` restores automatic
presentation. `delay(milliseconds)` continues presenting and processing window
events while it waits.

If the window is closed while Lua is executing, the runtime records the close
request, stops further presentation, allows the current Lua call or script to
finish, and then releases its Lua and Raylib resources normally.

Use `-o`/`--output` to export the completed canvas before the window enters its
display loop:

```sh
./build/ptrl -o drawing.png ../lua/samples/polygons.lua
```

## Planned non-interactive mode

The `0.3.0` runtime will add a headless/export mode for automation. It will
create the graphics context without showing a normal window, execute the Lua
program, export the canvas to the requested path, and exit. This mode will not
wait for user input after the program finishes.

## Tests

Build and run the automated tests with:

```sh
make build
ctest --test-dir build --output-on-failure
```

Rendering quality and platform-specific presentation are verified separately
with the Lua programs under `test/` and the existing samples under `../lua/`.
