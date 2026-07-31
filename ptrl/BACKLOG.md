# 0.3.0 (matrix-goose)

## Release goal

Replace the Skia/Qt implementation with a focused Raylib-based PicoTurtle
runtime that supports the established public Lua graphics API without carrying
forward the IDE, editor, or Qt application.

The release should run PicoTurtle Lua programs from a user's preferred editor,
display their output in a responsive Raylib window, and support image export.

All items below are in scope for this release. Before tagging the release, each
item must either be completed or deliberately moved to a later release backlog.

## Definition of done

- The Raylib runtime builds reproducibly on macOS through the existing
  `ptrl/Makefile` and vcpkg setup.
- Existing PicoTurtle Lua programs can run without depending on Qt or Skia.
- Every public Lua method from the original PicoTurtle implementation is either
  supported or explicitly documented as deprecated with a replacement.
- The original sample suite is used as the compatibility and regression suite.
- Static drawings can be exported to PNG.
- Animated programs remain responsive while painting or delaying.
- Core state and movement behaviour is covered by automated tests.

## Milestones

- [x] Milestone 1: Establish a sound Raylib runtime and canvas
  - [x] Add a runtime/canvas structure that owns the Raylib window and
        persistent `RenderTexture2D`.
  - [x] Associate each turtle with the shared runtime canvas.
  - [x] Initialize Raylib and the canvas before initializing and executing Lua.
  - [x] Route all drawing operations through valid `BeginTextureMode` and
        `EndTextureMode` scopes.
  - [x] Present the persistent canvas texture in a responsive window loop.
  - [x] Define clean shutdown and release all Raylib resources.
  - [x] Support multiple turtles drawing on the shared canvas.
  - [x] Decide and document normal windowed and non-interactive export
        behaviour.
  - [x] Remove the experimental drawing harness from the production execution
        path.

- [ ] Milestone 2: Restore core turtle and Lua API compatibility
  - [ ] Rewrite Lua bindings to read turtle userdata from stack index 1 and
        method arguments from subsequent indices.
  - [ ] Add consistent Lua argument-count, type, and range validation.
  - [x] Restore the original logical coordinate system, including Y-axis
        conversion between turtle and Raylib coordinates.
  - [x] Restore the initial position at the centre of the canvas.
  - [x] Restore the initial heading of 90 degrees, facing upward.
  - [x] Make `setpos`, `setx`, and `sety` draw when the pen is down.
  - [x] Implement and bind `getwidth`, `setwidth`, `getheight`, and `setheight`.
  - [x] Implement and bind `canvas_size`.
  - [x] Implement and bind `penup`/`pu` and `pendown`/`pd`.
  - [x] Implement and bind `penwidth`/`pw`.
  - [x] Implement and bind named and RGB forms of `pencolor`/`pc`.
  - [x] Implement and bind `home`, `clear`, `reset`, and the compatibility
        no-op `stop`.
  - [x] Implement and bind `forward`/`fd` and `back`/`bk`.
  - [x] Implement and bind `setpos`, `setx`, `sety`, `getx`, and `gety`.
  - [x] Implement and bind `left`/`lt`, `right`/`rt`, and `heading`.
  - [x] Implement and bind `drawme`.
  - [ ] Verify `turtle_basic.lua`, `spiral.lua`, `tree.lua`,
        `sierpinski.lua`, and `polygons.lua`.

- [ ] Milestone 3: Add state, shapes, timing, and image export
  - [ ] Add deep-copy helpers for turtle location, colour, font, and state.
  - [ ] Add a dynamically sized saved-state stack to each turtle.
  - [ ] Implement and bind `save` and `restore`.
  - [ ] Implement the detached TurtleState Lua userdata and its `__gc` and
        `__tostring` behaviour.
  - [ ] Implement TurtleState accessors `x`, `y`, `a`, `r`, `g`, `b`,
        `heading`/`hd`, `pd`, and `pw`.
  - [ ] Implement and bind `circle` with the current pen colour and width.
  - [ ] Port and bind all `arc` forms: radius; radius and extent; radius,
        extent, and steps.
  - [ ] Track turtle creation time and bind `elapsed_time_ms`.
  - [ ] Implement PNG export from the render texture, including vertical
        orientation correction.
  - [ ] Bind PNG export as `export_img` and `snap`.
  - [ ] Verify `save_restore.lua` and exported image dimensions and
        orientation.

- [ ] Milestone 4: Add text and picture support
  - [ ] Define portable font behaviour for bundled fonts, font-file paths, and
        unknown family names.
  - [ ] Add a font cache with deterministic ownership and cleanup.
  - [ ] Load the glyph ranges required by the supported Unicode samples.
  - [ ] Implement and bind `font`.
  - [ ] Implement rotated text drawing at the turtle's logical position.
  - [ ] Implement and bind `filltext`.
  - [ ] Implement `stroketext` with documented compatibility behaviour.
  - [ ] Add TurtleImage Lua userdata with deterministic texture cleanup.
  - [ ] Implement and bind `loadpic`.
  - [ ] Implement `pic` for both an image userdata object and an image path.
  - [ ] Verify `fern.lua`, `text-demo.lua`, `draw_mandrill.lua`, and
        `draw_mandrill_move.lua`.

- [ ] Milestone 5: Restore presentation and animation semantics
  - [ ] Define when drawing operations update the visible window.
  - [ ] Implement and bind `enable_update` and `disable_update`.
  - [ ] Implement and bind `paint` as an immediate presentation and event-poll
        operation.
  - [ ] Implement and bind responsive `delay` without freezing the Raylib
        window.
  - [ ] Ensure long-running and animated Lua programs continue to process
        window-close events.
  - [ ] Define behaviour when the user closes the window while Lua is still
        executing.
  - [ ] Verify animation using `draw_mandrill_move.lua`.

- [ ] Milestone 6: Complete compatibility, tests, and release preparation
  - [ ] Choose a low-maintenance GIF encoding implementation.
  - [ ] Implement and bind `picoturtle.makegif`.
  - [ ] Verify `gif_basic.lua`.
  - [ ] Run maze generation as a large-canvas and text-rendering regression
        test.
  - [ ] Add unit tests for locations, colours, headings, movement, pen state,
        state copying, and save/restore.
  - [ ] Add Lua integration tests covering every public method and alias.
  - [ ] Add reference-image tests for representative static drawings.
  - [ ] Add resource-lifetime tests for turtles, saved states, fonts, images,
        the canvas, and Lua garbage collection.
  - [x] Add a clean-build check using `make genbuild` followed by `make build`.
  - [ ] Remove the Skia, Qt, and GraphicsMagick dependencies from the 0.3.0
        build.
  - [x] Decide whether to replace `coll` and `zclk` with minimal direct CLI
        argument parsing.
  - [ ] Update the README with 0.3.0 build, run, and editor-independent usage
        instructions.
  - [ ] Update the API documentation with any deliberate compatibility
        differences.
  - [ ] Move the original Skia/Qt implementation to a clearly documented
        legacy location or branch.
  - [ ] Verify macOS packaging and runtime resource paths.
  - [ ] Verify Windows and Linux builds, or explicitly move them to a later
        release backlog.
  - [ ] Tag and publish `v0.3.0` only after every remaining item above is
        complete or deliberately rescheduled.
