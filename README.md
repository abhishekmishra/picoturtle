# PicoTurtle 0.3.0

PicoTurtle is a Raylib-based runtime for drawing with Lua, codenamed
`matrix-goose` for the `0.3.0` release. It runs turtle programs written in any
code editor without requiring the former Qt IDE or Skia renderer.

## Build

The build uses CMake, vcpkg, and the root Makefile:

```sh
make genbuild
make build
```

## Run

Pass one Lua program to the executable. Run repository samples from the
repository root so their relative `res/` paths resolve correctly:

```sh
./build/picoturtle lua/samples/polygons.lua
```

Relative file paths used by a Lua program are resolved from PicoTurtle's
current working directory. For personal scripts, launch PicoTurtle from the
directory against which those paths were written.

## Fonts

PicoTurtle uses Raylib font loading and bundles Noto Sans under the SIL Open
Font License 1.1. The `font` method accepts a font name and pixel size:

```lua
t:font("Noto Sans", 32)
t:filltext("PicoTurtle")
```

`default`, `normal`, and legacy Latin family names such as `Calibri`,
`Consolas`, and `Courier` use the bundled Noto Sans font. A path to a `.ttf`
or `.otf` file loads that font directly. Other unavailable family names fall
back to Noto Sans so output remains portable.

The 0.3.0 text API supports Latin-1. Complex scripts such as Devanagari are
outside the release scope because Raylib does not provide the shaping needed
to render them correctly.

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
./build/picoturtle -o drawing.png lua/samples/polygons.lua
```

## Tests

Build and run the automated tests with:

```sh
make build
ctest --test-dir build --output-on-failure
```

Rendering quality and platform-specific presentation are verified separately
with the Lua programs under `test/` and the existing samples under `lua/`.
