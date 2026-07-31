# PicoTurtle Lua API

PicoTurtle runs a Lua file and creates a global turtle named `t`. Programs can
reuse it or create their own turtle:

```lua
local t = t or require("picoturtle").new()
```

Multiple turtles share one canvas.

## Coordinates and headings

The logical origin is at the bottom-left of the canvas. Positive X points
right and positive Y points up. A new or reset turtle starts at the canvas
centre with heading `90`, facing up.

Angles are measured in degrees:

- `0` points right.
- `90` points up.
- `180` points left.
- `270` points down.

## Movement

| Method | Description |
| --- | --- |
| `forward(distance)`, `fd(distance)` | Move forward, drawing when the pen is down. |
| `back(distance)`, `bk(distance)` | Move backward. |
| `left(angle)`, `lt(angle)` | Turn counter-clockwise. |
| `right(angle)`, `rt(angle)` | Turn clockwise. |
| `heading(angle)` | Set the absolute heading. |
| `setpos(x, y)` | Move to an absolute logical position. |
| `setx(x)` | Change only the X coordinate. |
| `sety(y)` | Change only the Y coordinate. |
| `getx()` | Return the logical X coordinate. |
| `gety()` | Return the logical Y coordinate. |
| `home()` | Move to the canvas centre without changing the other pen settings. |

`setpos`, `setx`, and `sety` draw when the pen is down.

## Pen and colour

| Method | Description |
| --- | --- |
| `penup()`, `pu()` | Lift the pen. |
| `pendown()`, `pd()` | Lower the pen. |
| `penwidth(width)`, `pw(width)` | Set a positive line width. |
| `pencolor(name)`, `pc(name)` | Set a named colour; returns `1` when found and `0` otherwise. |
| `pencolor(r, g, b)`, `pc(r, g, b)` | Set RGB components from `0` through `255`; returns `0`. |

## Canvas

| Method | Description |
| --- | --- |
| `getwidth()` | Return canvas width. |
| `setwidth(width)` | Set a positive canvas width. |
| `getheight()` | Return canvas height. |
| `setheight(height)` | Set a positive canvas height. |
| `canvas_size()` | Return `width, height`. |
| `canvas_size(width, height)` | Resize and return `width, height`. |
| `clear()` | Clear to white. |
| `clear(name)` | Clear to a named colour. |
| `clear(r, g, b)` | Clear to an RGB colour. |
| `reset()` | Clear and restore the initial location, heading, pen state, and width. |

Resizing clears the current canvas.

## Shapes

| Method | Description |
| --- | --- |
| `circle(radius)` | Draw a circle centred at the turtle without moving it. |
| `arc(radius)` | Draw a full circle using turtle movement. |
| `arc(radius, extent)` | Draw an arc with the given extent. |
| `arc(radius, extent, steps)` | Draw an arc using a positive number of line segments. |
| `drawme()` | Draw a small turtle marker at the current position and heading. |

## State

| Method | Description |
| --- | --- |
| `state()` | Return a detached snapshot of the current turtle state. |
| `save()` | Push a deep state copy onto this turtle's stack. |
| `restore()` | Restore and remove the most recently saved state; an empty stack is harmless. |
| `elapsed_time_ms()` | Return milliseconds since the turtle was created. |
| `stop()` | Compatibility no-op retained for older programs. |

A state snapshot supports:

```lua
local state = t:state()
print(state:x(), state:y())
print(state:r(), state:g(), state:b(), state:a())
print(state:heading(), state:hd())
print(state:pd(), state:pw())
print(state)
```

Snapshots remain unchanged when the turtle later moves or changes colour.

## Text

| Method | Description |
| --- | --- |
| `font(name, size)` | Select a font and positive pixel size. |
| `filltext(text)` | Draw rotated text at the turtle position. |
| `stroketext(text)` | Compatibility form currently rendered like `filltext`. |

PicoTurtle bundles Noto Sans and supports Latin-1. `default`, `normal`, and
unavailable family names use the bundled font. A path to a `.ttf` or `.otf`
file loads that file. Complex-script shaping is outside the 0.3.0 scope.

## Pictures

```lua
local image = t:loadpic("res/images/mandrill.png")
t:pic(image)
t:pic("res/images/mandrill.png")
```

`loadpic(path)` returns a reusable image object. `pic` accepts that object or a
path. The turtle position is the image's top-left anchor after conversion from
logical coordinates. Relative paths are resolved from PicoTurtle's current
working directory.

## Presentation and animation

| Method | Description |
| --- | --- |
| `disable_update()` | Stop automatically presenting drawing operations. |
| `enable_update()` | Restore automatic presentation and present immediately. |
| `paint()` | Present the complete current canvas immediately. |
| `delay(milliseconds)` | Wait while continuing to present and process window events. |

For flicker-free animation, disable automatic updates, draw one complete frame,
call `paint`, and then delay:

```lua
t:disable_update()

for frame = 1, 20 do
    t:clear()
    -- draw the complete frame
    t:paint()
    t:delay(50)
end

t:enable_update()
```

## PNG export

`export_img(path)` and its alias `snap(path)` export the complete canvas as a
PNG and return the path:

```lua
local filename = t:snap("drawing.png")
```

The module-level `picoturtle.makegif` helper from 0.2.x was removed. Assemble
exported PNG frames with an external animation tool when required.

## Errors

Incorrect argument counts or types, invalid RGB components, non-positive
dimensions or pen widths, negative delays, zero radii, and non-positive arc
step counts raise Lua errors. Missing images and failed PNG exports also raise
errors.

See [0.3.0 compatibility notes](../API_COMPATIBILITY.md) for deliberate
differences from the former Skia/Qt implementation.
