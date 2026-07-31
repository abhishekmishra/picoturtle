# PicoTurtle 0.3.0 API compatibility

PicoTurtle 0.3.0 keeps the public Lua turtle API from the 0.2.x Skia/Qt
implementation while replacing its renderer and desktop application with a
focused Raylib runtime.

## Module API

| API | 0.3.0 status |
| --- | --- |
| `picoturtle.new()` | Supported |
| `picoturtle.makegif(images, path)` | Removed; export PNG snapshots and assemble animations with an external tool |

## Turtle methods

All of these methods are supported:

| Area | Methods and aliases |
| --- | --- |
| Canvas | `getwidth`, `setwidth`, `getheight`, `setheight`, `canvas_size`, `clear`, `reset` |
| Pen | `penup`/`pu`, `pendown`/`pd`, `penwidth`/`pw`, `pencolor`/`pc` |
| Movement | `forward`/`fd`, `back`/`bk`, `setpos`, `setx`, `sety`, `getx`, `gety`, `left`/`lt`, `right`/`rt`, `heading`, `home` |
| Shapes | `drawme`, `circle`, `arc` |
| State | `state`, `save`, `restore`, `elapsed_time_ms`, `stop` |
| Presentation | `enable_update`, `disable_update`, `paint`, `delay` |
| Export and images | `export_img`/`snap`, `loadpic`, `pic` |
| Text | `font`, `filltext`, `stroketext` |

## TurtleState methods

Detached state snapshots support `x`, `y`, `a`, `r`, `g`, `b`,
`heading`/`hd`, `pd`, and `pw`, plus `tostring(state)`.

## Deliberate differences

- `state()` returns a deep, detached snapshot. The older binding exposed the
  turtle's internally owned state through Lua userdata, which made ownership
  and garbage collection unsafe.
- Method argument counts, numeric ranges, positive dimensions, positive pen
  widths, and positive arc step counts are validated consistently. Invalid
  calls raise a Lua error instead of being silently misinterpreted.
- `font(name, size)` uses bundled Noto Sans, a supplied font-file path, or a
  portable fallback. Platform font-family discovery is not provided.
- Text support is limited to Latin-1 in 0.3.0. Raylib does not provide the
  complex-script shaping required for correct Devanagari rendering.
- `stroketext` currently renders through the same Raylib text path as
  `filltext`; true outlined glyph rendering is not provided.
- Image positions retain the original top-left image anchoring after conversion
  from PicoTurtle's bottom-left logical coordinate system.
- The Qt IDE, editor, console, and embedded documentation viewer are removed.
  Lua programs are run from the `picoturtle` command-line executable.
